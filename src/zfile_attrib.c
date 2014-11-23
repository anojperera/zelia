/* implementation of the attribute class */

#include <string.h>
#include <libxml/HTMLparser.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "zVar.h"
#include "zfile_attrib.h"

#define ZFILE_ATTRIB_PROP_1 "id"

static int _zfile_attrib_count_field_array(zfile_attrib* obj, struct zfile_attrib_field* array);
static int _zfile_attrib_itr_node(zfile_attrib* obj, xmlNode* root);
static char* _zfile_attrib_read_contents(const char* file_path, char** buff, size_t* sz);
static int _zfile_attrib_delete_field_array(zfile_attrib* obj);

/* constructor */
zfile_attrib* zfile_attrib_new(zfile_attrib* obj, xmlDocPtr doc)
{
    if(obj == NULL)
	{
	    obj = (zfile_attrib*) malloc(sizeof(zfile_attrib));
	    obj->_int_flg = ZELIA_CONSTRUCTED;
	    ZELIA_LOG_MESSAGE("zfile_attrib object created");
	}
    else
	{
	    obj->_int_flg = ZELIA_INITIALISED;
	    ZELIA_LOG_MESSAGE("zfile_attrib object is initialised");
	}

    obj->field_count = 0;
    obj->attrib_tbl_name = NULL;
    obj->field_array = NULL;
    obj->field_cr_flg = 0;
    obj->xml_doc = doc;
    obj->update_complete_callback = NULL;
    obj->ext_obj = NULL;
    obj->child = NULL;

    return obj;
}

/* destructor */
void zfile_attrib_delete(zfile_attrib* obj)
{
    if(obj == NULL)
	return;

    obj->attrib_tbl_name = NULL;
    obj->field_array = NULL;
    obj->xml_doc = NULL;
    obj->update_complete_callback = NULL;
    obj->ext_obj = NULL;
    obj->child = NULL;

    /* if the field array was internally created we delete it */
    _zfile_attrib_delete_field_array(obj);
    
    /* free object if it was created */
    if(obj->_int_flg == ZELIA_CONSTRUCTED)
	free(obj);

    return;
}

/* set field array and update xml */
int zfile_attrib_set_field_array(zfile_attrib* obj, struct zfile_attrib_field* array, size_t sz)
{
    xmlNodePtr _root_node = NULL;
    
    /* check for null */
    if(obj == NULL)
	return ZELIA_NULL;

    /* check if xml document pointer was set */
    if(obj->xml_doc == NULL)
	return ZELIA_ATTRIB_ERROR;

    /* check array argument */
    if(array != NULL && obj->field_array == NULL)
	obj->field_array = array;
    else if(array == NULL && obj->field_array == NULL)
	return ZELIA_ATTRIB_ERROR;

    /* if the size is greater than zero we set the size */
    if(sz > 0)
	obj->field_count = sz;
    else if(array)
	{
	    ZELIA_LOG_MESSAGE("zfile_attrib array count was passed zero, obtaining array count");
	    _zfile_attrib_count_field_array(obj, array);
	}


    /* get root node */
    _root_node = xmlDocGetRootElement(obj->xml_doc);

    /* call method to iterate over the struct and set the values */
    ZELIA_LOG_MESSAGE("zfile_attrib searching svg to apply attributes");
    _zfile_attrib_itr_node(obj, _root_node);
    
    /* if the callback was set we call them with the external pointer */
    if(obj->update_complete_callback)
	{
	    ZELIA_LOG_MESSAGE("zfile_attrib calling callback to indicate apply complete");
	    obj->update_complete_callback(obj->ext_obj);
	}
    return ZELIA_OK;
}

int zfile_attrib_parse_attrib_xml(zfile_attrib* obj, const char* file_path)
{
    size_t _buff_sz = 0;
    char* _buff;						/* temporary buffer */
    xmlDocPtr _xml;						/* xml document pointer */
    xmlNodePtr _root_node = NULL;
    xmlNodePtr _node = NULL;
    xmlNodePtr _t_node = NULL;
    unsigned long _count = 0;
    unsigned long _cnt = 0;					/* general purpose counter */

    struct zfile_attrib_field* _field_itr = NULL;		/* field iterator */
    
    xmlChar* _prop_val;
    xmlChar* _value;
    
    /* check for object */
    if(obj == NULL)
	return ZELIA_NULL;

    /* read the file and load it into buffer */
    if(!_zfile_attrib_read_contents(file_path, &_buff, &_buff_sz))
	return ZELIA_ATTRIB_ERROR;
    
    ZELIA_LOG_MESSAGE("zfile_attrib parsing xml");
    
    xmlInitParser();
    _xml = xmlParseMemory(_buff, _buff_sz);

    if(!_xml)
	{
	    ZELIA_LOG_MESSAGE("zfile_attrib unable to parse the xml");
	    free(_buff);
	    return ZELIA_ATTRIB_ERROR;
	}

    ZELIA_LOG_MESSAGE("zfile_attrib iterating through the document");
    _root_node = xmlDocGetRootElement(_xml);

    /* get a count */
    _count = xmlChildElementCount(_root_node);
    obj->field_count = (size_t) _count;
    
    if(_count == 0)
	{
	    ZELIA_LOG_MESSAGE("zfile_attrib xml element count is zero, therefore we bailing");
	    xmlFreeDoc(_xml);
	    xmlCleanupParser();
	    return ZELIA_ATTRIB_ERROR;
	}

    ZELIA_LOG_MESSAGE("zfile_attrib creating a keys - value pair array");
    obj->field_count = _count;
    obj->field_array = (struct zfile_attrib_field*) calloc(sizeof(struct zfile_attrib_field), _count);
    obj->field_cr_flg = 1;

    /*
     * We set the field array iterator to set the values.
     */
    _field_itr = obj->field_array;    
    
    _node = xmlFirstElementChild(_root_node);
    while(_node)
	{
	    _t_node = _node;
	    _node = xmlFirstElementChild(_node);
	    if(_node)
		continue;
	    else
		_node = _t_node;
	    ZELIA_LOG_MESSAGE("zfile_attrib getting property and values from xml");
	    _prop_val = xmlGetProp(_node, (xmlChar*) ZFILE_ATTRIB_PROP_1);
	    _value = xmlNodeGetContent(_node);

	    _field_itr->field_id = (char*) _prop_val;
	    _field_itr->field_value = (char*) _value;

	    if(++_cnt >= _count)
		break;
	    
	    _field_itr++;
	    _node = xmlNextElementSibling(_node);
	}

    xmlFreeDoc(_xml);
    xmlCleanupParser();
    
    ZELIA_LOG_MESSAGE("zelia_attrib complete parsing the file");
    return ZELIA_OK;
}

/*============================== Private Methods ====================*/
static int _zfile_attrib_count_field_array(zfile_attrib* obj, struct zfile_attrib_field* array)
{
    /* object checking is not required */
    struct zfile_attrib_field* _array = array;
    
    obj->field_count = 0;
    while(1)
	{
	    /* break from array if the pointer or elements are NULL */
	    if(_array == NULL || (_array->field_id == NULL && _array->field_value == NULL))
	       break;

	       obj->field_count++;
	       _array++;
	}
    
    ZELIA_LOG_MESSAGE_WITH_INT("zfile_attrib array element count - ", (int) obj->field_count);
    return ZELIA_OK;
}


/* iterate over the nodes and if id is the same then we set the value */
static int _zfile_attrib_itr_node(zfile_attrib* obj, xmlNodePtr root)
{
    /* no need to check for object pointer */
    xmlNodePtr _node_itr = root;
    xmlNodePtr _node_child_itr = NULL;
    xmlChar* _prop_val = NULL;
    xmlChar* _new_val = NULL;
    struct zfile_attrib_field* _field_array = NULL;
    unsigned int _i = 0;
    
    while(_node_itr)
	{
	    ZELIA_LOG_MESSAGE("iterating through xml elements");
	    _node_child_itr = _node_itr;
	    
	    /* get id property */
	    _prop_val = xmlGetProp(_node_itr, (xmlChar*) ZFILE_ATTRIB_PROP_1);
	    if(_prop_val != NULL)
		{
		    /* for each node we go through the array and figure out if id we are after exists */
		    for(_i = 0, _field_array = obj->field_array;
			_i < obj->field_count;
			_i++, _field_array++)
			{
			    if(strcmp((char*) _prop_val, _field_array->field_id) == 0)
				{
				    ZELIA_LOG_MESSAGE("found property setting value");
				    _new_val = xmlEncodeEntitiesReentrant(obj->xml_doc, (xmlChar*) _field_array->field_value);
				    if(_new_val)
					{
					    xmlNodeSetContent(_node_itr, _new_val);
					    xmlFree(_new_val);
					}

				    break;
				}
			}
		    xmlFree(_prop_val);
		}
	    
	    _node_itr = xmlFirstElementChild(_node_itr);
	    if(_node_itr)
		{
		    ZELIA_LOG_MESSAGE("zfile_attrib child found, stepping into child");
		    ZELIA_LOG_MESSAGE_WITH_STR("zfile_attrib parent node - ", (char*) _node_itr->name);
		    _zfile_attrib_itr_node(obj, _node_itr);
		}
	    
	    _node_itr = _node_child_itr;
	    ZELIA_LOG_MESSAGE("zfile_attrib looking at next sibling");
	    ZELIA_LOG_MESSAGE_WITH_STR("zfile_attrib current sibling - ", (char*) _node_itr->name);	    
	    _node_itr = xmlNextElementSibling(_node_itr);

	}

    return ZELIA_OK;
}


static char* _zfile_attrib_read_contents(const char* file_path, char** buff, size_t* sz)
{
    int r_fd = 0;
    struct stat tmp_stat;								/* struct for holding the template file stat */    
    
    ZELIA_LOG_MESSAGE_WITH_STR("zfile_attrib check file exist", file_path);
    if(access(file_path, F_OK))
	{
	    ZELIA_LOG_MESSAGE("zfile_attrib file does not exist");
	    return NULL;
	}

    /* open file */
    r_fd = open(file_path, O_RDONLY);
    if(r_fd == -1)
	{
   	    ZELIA_LOG_MESSAGE("zfile_attrib errors occured while reading the template");
	    return NULL;
	}

    /* check stats for the file to obtain the size */
    ZELIA_LOG_MESSAGE("checking file stats");
    if(fstat(r_fd, &tmp_stat))
	{
	    ZELIA_LOG_MESSAGE("zfile_attrib unable to get tempalate file size");
	    ZELIA_LOG_MESSAGE("zfile_attrib unable closing template file and write file");
	    close(r_fd);
	    return NULL;
	}

    *sz = tmp_stat.st_size+1;
    *buff = (char*) malloc(tmp_stat.st_size+1);

    if(!read(r_fd, (void*) *buff, tmp_stat.st_size))
	{
	    ZELIA_LOG_MESSAGE("zfile_attrib unable to read the file");
	    free(*buff);
	    close(r_fd);

	    return NULL;
	}

    /* close the file descriptor */
    close(r_fd);
    ZELIA_LOG_MESSAGE("zfile_attrib file read and loaded in to temporary buffer");
    
    return *buff;
}

/* helper method for deleting the field array */
static int _zfile_attrib_delete_field_array(zfile_attrib* obj)
{
    size_t _cnt = 0;
    struct zfile_attrib_field* _field_itr = obj->field_array;
    
    while(obj->field_cr_flg && _field_itr)
	{
	    xmlFree(_field_itr->field_id);
	    xmlFree(_field_itr->field_value);

	    _field_itr++;
	    if(++_cnt >= obj->field_count)
		break;
	}

    if(obj->field_cr_flg)
	free(obj->field_array);

    obj->field_array = NULL;
    return ZELIA_OK;
}
