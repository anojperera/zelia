#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>

#include "zVar.h"
#include "zfile.h"
#include "zfile_attrib.h"

#include "zelia_parser.h"
#include "zdevice.h"
#include "zgeneric.h"
#include "zgenerics.h"
#include "zbase.h"
#include "zterminal.h"
#include "zterminals.h"
#include "zjb.h"
#include "zgland.h"
#include "zglands.h"
#include "ztable.h"
#include "znote.h"
#include "znotes.h"
#include "zleader.h"
#include "zlabel.h"

#define ZELIA_XML_TSTRUCT_ALIGN 8


static int _read_file(const char* temp_path, char** buff, size_t* size);
static zelia_object_types _get_type(const char* tag_name);
int _main_loop(xmlNodePtr node, struct _zparser* parser);

static void _delete_helper(void* data);

int _create_file_object(xmlNodePtr node, struct _zparser* parser);
int _create_attrib_object(xmlNodePtr node, struct _zparser* parser);
int _create_notes_object(xmlNodePtr node, struct _zparser* parser);
int _create_table_object(xmlNodePtr node, struct _zparser* parser);
int _create_jb_object(xmlNodePtr node, struct _zparser* parser);
int _create_leader_object(xmlNodePtr node, struct _zparser* parser);
int _create_label_object(xmlNodePtr node, struct _zparser* parser);

inline __attribute__ ((always_inline)) static int _create_table_object_dim(xmlNodePtr node, zgeneric* object);
inline __attribute__ ((always_inline)) static int _create_table_object_content(xmlNodePtr node, const xmlChar* content, zgeneric* object);
inline __attribute__ ((always_inline)) static int _create_jb_object_helper(xmlNodePtr node, void* object);
inline __attribute__ ((always_inline)) static int _create_jb_object_terminals_helper(xmlNodePtr node, zgeneric* object);
inline __attribute__ ((always_inline)) static int _create_jb_object_gland_helper(xmlNodePtr node, zgeneric* object);



#define _finalise_parser zelia_xml_finalise

void* zelia_xml_parse_file(const char* xml_path, void* parser);
int zelia_xml_finalise(struct _zparser* parser);

/* int zelia_parse_buffer(const char* buff, const size_t sz); */

/* parse xml file and create objects */
void* zelia_xml_parse_file(const char* xml_path, void* parser)
{
    char* _buff = NULL;					/* buffer to hold the file contents */
    size_t _sz = 0;					/* buffer file size */

    xmlDocPtr _xml_doc = NULL;				/* xml document pointer */
    xmlNodePtr _root = NULL;
    struct _zparser* obj = NULL;
    
    /* check arguments */
    ZCHECK_OBJ_INT(xml_path);
    
    if(parser == NULL)
	{
	    ZCONSTRUCTOR(obj, struct _zparser);
	}
    else
	obj = (struct _zparser*) parser;

    /* read the file */
    if(_read_file(xml_path, &_buff, &_sz) != ZELIA_OK)
	return NULL;


    /* initialise object array */
    blist_new(&obj->object_array, _delete_helper);

    if(!zfile_new(&obj->file))
	return NULL;

    /* toggle the force overwrite mode */
    zfile_toggle_overwrite(&obj->file);

    /* parse the document */
    xmlInitParser();

    _xml_doc = xmlParseMemory(_buff, _sz);

    if(_xml_doc == NULL)
	{
	    ZELIA_LOG_MESSAGE("zelia_xml unable to parse the document");
	    goto clean_up;
	}

    _root = xmlDocGetRootElement(_xml_doc);

    _main_loop(xmlFirstElementChild(_root), obj);
    
    xmlFreeDoc(_xml_doc);
    xmlCleanupParser();

 clean_up:

    if(_buff != NULL && _sz > 0)
	free(_buff);
    _buff = NULL;
    _sz = 0;
    
    if(ZDESTRUCTOR_CHECK)
	{
	    _finalise_parser(obj);
	    free(obj);
	}
    else
	return obj;
    
    return NULL;
}



/*=================================== Private Methods ===================================*/

/* read file contents in to a buffer */
static int _read_file(const char* temp_path, char** buff, size_t* size)
{
    int _fd = -1;				/* file descriptor */
    struct stat _fstat;				/* file stat struct */

    /* check arguments */
    ZCHECK_OBJ_INT(temp_path);
    ZCHECK_OBJ_INT(buff);
    ZCHECK_OBJ_INT(size);

    ZELIA_LOG_MESSAGE("zelia_xml checking if file exists");
    if(access(temp_path, F_OK))
	{
	    ZELIA_LOG_MESSAGE_WITH_STR("file doest not exist ", temp_path);
	    return ZELIA_PARSER_ERROR;
	}

    ZELIA_LOG_MESSAGE("zelia_xml opening file");
    _fd = open(temp_path, O_RDONLY);
    if(_fd == -1)
	{
	    ZELIA_LOG_MESSAGE("zelia_xml unable to open the file");
	    return ZELIA_PARSER_ERROR;
	}

    /* get file size */
    ZELIA_LOG_MESSAGE("zelia_xml getting file stat");
    if(fstat(_fd, &_fstat))
	{
	    ZELIA_LOG_MESSAGE("zelia_xml unable to get file stat");
	    ZELIA_LOG_MESSAGE("zelia_xml closing file descriptor");
	    close(_fd);
	    return ZELIA_PARSER_ERROR;
	}

    /* allocate size and buffer */
    *size = (size_t) _fstat.st_size + 1;
    *buff = (char*) malloc((*size) * sizeof(char));

    /* read the contents into the buffer */

    if(!read(_fd, (void*) (*buff), _fstat.st_size))
	{
	    ZELIA_LOG_MESSAGE("unable to read the file");

	    /* free the alocated buffer */
	    *size = 0;
	    free(*buff);
	    *buff = NULL;
	    close(_fd);
	    return ZELIA_PARSER_ERROR;
	}

    /* close file descriptor and return success */
    ZELIA_LOG_MESSAGE("file contents read successfully");
    close(_fd);
    return ZELIA_OK;
}

/* method for returning the enumerated type of object from xml tag name */
static zelia_object_types _get_type(const char* tag_name)
{
    /* compare strings and return the enumerated type */
    if(strcmp(tag_name, ZPARSER_FILE) == 0)
	return zfile_type;
    else if(strcmp(tag_name, ZPARSER_SHEET_ATTRIB) == 0)
	return zfile_attrib_type;
    else if(strcmp(tag_name, ZPARSER_BASE) == 0)
	return zbase_type;
    else if(strcmp(tag_name, ZPARSER_GLAND) == 0)
	return zgland_type;
    else if(strcmp(tag_name, ZPARSER_JB) == 0)
	return zjb_type;
    else if(strcmp(tag_name, ZPARSER_TERMINAL) == 0)
	return zterminal_type;
    else if(strcmp(tag_name, ZPARSER_NOTE) == 0)
	return znote_type;
    else if(strcmp(tag_name, ZPARSER_TABLE_TCELL) == 0)
	return ztcell_type;
    else if(strcmp(tag_name, ZPARSER_TABLE_ROW) == 0)
	return ztrow_type;
    else if(strcmp(tag_name, ZPARSER_TABLE) == 0)
	return ztable_type;
    else if(strcmp(tag_name, ZPARSER_GENERIC) == 0)
	return zgeneric_type;
    else if(strcmp(tag_name, ZPARSER_GENERICS) == 0)
	return zgenerics_type;
    else if(strcmp(tag_name, ZPARSER_GLANDS) == 0)
	return zglands_type;
    else if(strcmp(tag_name, ZPARSER_NOTES) == 0)
	return znotes_type;
    else if(strcmp(tag_name, ZPARSER_TERMINALS) == 0)
	return zterminals_type;
    else if(strcmp(tag_name, ZPARSER_TABLE_ROWS) == 0)
	return ztrows_type;
    else if(strcmp(tag_name, ZPARSER_TABLE_CELLS) == 0)
	return ztcells_type;
    else if(strcmp(tag_name, ZPARSER_ARROW) == 0)
	return zarrow_type;
    else if(strcmp(tag_name, ZPARSER_LEADER) == 0)
	return zleader_type;
    else if(strcmp(tag_name, ZPARSER_LABEL) == 0)
	return zlabel_type;
    return zunknown_type;
}

/* main loop handler */
int _main_loop(xmlNodePtr node, struct _zparser* parser)
{
    unsigned int _flg = 0;					/* flag to indicate attrib and file were found */
    xmlNodePtr _node = node;					/* xml node */

    /* look for the file node first */
    zelia_object_types _type;

    ZELIA_LOG_MESSAGE("zelia_xml main loop processing nodes");
    ZELIA_LOG_MESSAGE("zelia_xml looking for file node");
    while(_node)
	{
	    if(_get_type((const char*) _node->name) == zfile_type)
		{
		    _create_file_object(_node, parser);
		    _flg++;
		    break;
		}
	    _node = xmlNextElementSibling(_node);
	}

    /* if the flag is zero file node not found exit here */
    if(_flg == 0)
	{
	    ZELIA_LOG_MESSAGE("zelia_xml file node not found exiting");
	    return ZELIA_PARSER_ERROR;
	}


    _node = node;
    while(_node)
	{
	    _type = _get_type((const char*) _node->name);
	    switch(_type)
		{
		case zfile_attrib_type:
		    _create_attrib_object(_node, parser);
		    break;
		case znotes_type:
		    _create_notes_object(_node, parser);
		    break;
		case ztable_type:
		    _create_table_object(_node, parser);
		    break;
		case zjb_type:
		    _create_jb_object(_node, parser);
		    break;
		case zleader_type:
		    _create_leader_object(_node, parser);
		    break;
		case zlabel_type:
		    _create_label_object(_node, parser);
		    break;
		default:
		    break;
		};
	    _node = xmlNextElementSibling(_node);
	}

    return ZELIA_OK;
}

int _create_file_object(xmlNodePtr node, struct _zparser* parser)
{
    char* _save_path = NULL;
    char* _template_path = NULL;
    char* _sheet_size = NULL;
    zSheets _sheet_type = zSheetA4_Portrait;

    xmlNodePtr _child = NULL;
    int stat = 0;

    /* get first child node */
    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    if(strcmp((const char*) _child->name, ZPARSER_TEMPLATE_PATH) == 0)
		_template_path = (char*) xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_SAVE_PATH) == 0)
		_save_path = (char*) xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_SHEET_SIZE) == 0)
		_sheet_size = (char*) xmlNodeGetContent(_child);

	    _child = xmlNextElementSibling(_child);
	}

    /* if the paths were set we create the file object */
    stat = zfile_create_file_from_template(&parser->file, _template_path, _save_path);
    if(stat != ZELIA_OK)
	ZELIA_LOG_MESSAGE("errors occured while creating a new file");

    if(strcmp(_sheet_size, ZPARSER_SHEET_A4_PORT) == 0)
	_sheet_type = zSheetA4_Portrait;
    else if(strcmp(_sheet_size, ZPARSER_SHEET_A4_LAND) == 0)
	_sheet_type = zSheetA4_Landscape;
    else if(strcmp(_sheet_size, ZPARSER_SHEET_A3_PORT) == 0)
	_sheet_type = zSheetA3_Portrait;
    else
	_sheet_type = zSheetA3_Landscape;

    zdevice_new2(_sheet_type, 0, &parser->device);

    /* free memory */
    free(_template_path);
    free(_save_path);
    free(_sheet_size);
    return ZELIA_OK;
}

/* create attribute object */
int _create_attrib_object(xmlNodePtr node, struct _zparser* parser)
{
    /* get first child node */
    zfile_attrib_new(&parser->attrib, zfile_get_xmldoc_ptr(&parser->file));
    zfile_attrib_set_ext_obj(&parser->attrib, (void*) &parser->file);
    zfile_attrib_add_complete_hook(&parser->attrib, zfile_get_callback_hook(&parser->file));
    zfile_attrib_parse_from_node(&parser->attrib, node);
    zfile_attrib_set_field_array(&parser->attrib, NULL, 0);

    return ZELIA_OK;
}

/* create notes object */
int _create_notes_object(xmlNodePtr node, struct _zparser* parser)
{
    char* _widths=NULL, *_xs=NULL, *_ys=NULL;
    double _x=0.0, _y=0.0, _width=0.0;
    char* _t_buff;
    struct _zobject* _obj = NULL;

    xmlNodePtr _child = NULL;

    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    if(strcmp((const char*) _child->name, ZPARSER_COORD_X) == 0)
		_xs = (char*) xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_COORD_Y) == 0)
		_ys = (char*) xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_COLUMN_WIDTH_ATTRIB) == 0)
		_widths = (char*) xmlNodeGetContent(_child);

	    /* when all three parameters are set, we come out of the loop */
	    if(_xs && _ys && _widths)
		break;
	    _child = xmlNextElementSibling(_child);
	}

    /* convert the values to doubles */
    _x = atof(_xs);
    _y = atof(_ys);
    _width = atof(_widths);

    /* create notes object collection */
    _obj = (struct _zobject*) malloc(sizeof(struct _zobject));
    _obj->type = zobject_cols;

    _obj->data._c = znotes_new(NULL, &parser->device, _x, _y, _width);

    /* set the reference flag */
    if(!zgenerics_get_ref_flg(_obj->data._c))
	{
	    zgenerics_toggle_ref_flg(_obj->data._c);
	}

    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    /* iterater through the array again and add notes to the collection */
	    if(strcmp((const char*) _child->name, ZPARSER_TITLE) == 0)
		{
		    _t_buff = (char*) xmlNodeGetContent(_child);
		    znotes_set_title(Z_NOTES(_obj->data._c), _t_buff);
		}
	    else if(strcmp((const char*) _child->name, ZPARSER_NOTE) == 0)
		{
		    _t_buff = (char*) xmlNodeGetContent(_child);
		    znotes_add(Z_NOTES(_obj->data._c), _t_buff);
		}
	    _child = xmlNextElementSibling(_child);
	}

    /* call draw method */
    zgenerics_draw(_obj->data._c);


    /* push to the collection */
    blist_add_from_end(&parser->object_array, (void*) _obj);

    if(_xs)
	free(_xs);
    if(_ys)
	free(_ys);
    if(_widths)
	free(_widths);
    return ZELIA_OK;
}

/* create table object */
int _create_table_object(xmlNodePtr node, struct _zparser* parser)
{
    xmlChar* _content = NULL, *_rows = NULL, *_cols = NULL;
    double _dim_dbl = 0.0;
    int _num_cols = 0, _num_rows = 0;
    struct _zobject* _obj = NULL;

    xmlNodePtr _child = NULL;


    /* create notes object collection */
    _obj = (struct _zobject*) malloc(sizeof(struct _zobject));
    _obj->type = zobject_item;

    /* create the table object */
    _obj->data._i = ztable_new(NULL);

    if(_obj->data._i == NULL)
	{
	    free(_obj);
	    return ZELIA_NULL;
	}
    zgeneric_set_device(_obj->data._i, &parser->device);
    zgeneric_set_default_dev_context(_obj->data._i);

    /* set the reference flag this is so that retrieved values of xml to be freed */
    if(!zgenerics_get_ref_flg(_obj->data._c))
	{
	    zgenerics_toggle_ref_flg(_obj->data._c);
	}


    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    _content = xmlNodeGetContent(_child);
	    if(_content != NULL)
		_dim_dbl = atof((char*) _content);

	    if(strcmp((const char*) _child->name, ZPARSER_COORD_X) == 0)
		zbase_set_base_coords_x(Z_BASE(_obj->data._i), _dim_dbl);
	    else if(strcmp((const char*) _child->name, ZPARSER_COORD_Y) == 0)
		zbase_set_base_coords_y(Z_BASE(_obj->data._i), _dim_dbl);
	    else if(strcmp((const char*) _child->name, ZPARSER_ROW_HEIGHT_ATTRIB) == 0)
		zbase_set_height(Z_BASE(_obj->data._i), _dim_dbl);
	    else if(strcmp((const char*) _child->name, ZPARSER_COLUMN_WIDTH_ATTRIB) == 0)
		zbase_set_width(Z_BASE(_obj->data._i), _dim_dbl);
	    else if(strcmp((const char*) _child->name, ZPARSER_TABLE_ROWS) == 0 && _rows == NULL)
		_rows = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_TABLE_COLUMNS) == 0 && _cols == NULL)
		_cols = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_TABLE_COLUMN) == 0)
		_create_table_object_dim(_child, _obj->data._i);
	    else if(strcmp((const char*) _child->name, ZPARSER_CONTENT) == 0)
		_create_table_object_content(_child, _content, _obj->data._i);

	    /* set numbeer of rows and columns */
	    if(_cols && _rows)
		{
		    _num_rows = atoi((char*) _rows);
		    _num_cols = atoi((char*) _cols);

		    ztable_set_rows_and_cols(Z_TABLE(_obj->data._i), _num_rows, _num_cols);

		    xmlFree(_rows);
		    xmlFree(_cols);

		    _cols = NULL;
		    _rows = NULL;
		}

	    /* free copy */
	    if(_content)
		xmlFree(_content);

	    _content = NULL;
	    _child = xmlNextElementSibling(_child);
	}

    /* call draw method */
    zgeneric_draw(_obj->data._i);

    /* push to the collection */
    blist_add_from_end(&parser->object_array, (void*) _obj);

    return ZELIA_OK;
}

/* create junction boxes */
int _create_jb_object(xmlNodePtr node, struct _zparser* parser)
{
    xmlNodePtr _child = NULL;
    struct _zobject* _obj = NULL;
    struct _jb_dims
    {
	double x __attribute__ ((aligned (ZELIA_XML_TSTRUCT_ALIGN)));
	double y __attribute__ ((aligned (ZELIA_XML_TSTRUCT_ALIGN)));
	double width __attribute__ ((aligned (ZELIA_XML_TSTRUCT_ALIGN)));
	double height __attribute__ ((aligned (ZELIA_XML_TSTRUCT_ALIGN)));
	double depth __attribute__ ((aligned (ZELIA_XML_TSTRUCT_ALIGN)));
	double radius __attribute__ ((aligned (ZELIA_XML_TSTRUCT_ALIGN)));
	double angle __attribute__ ((aligned (ZELIA_XML_TSTRUCT_ALIGN)));
    } _jb_dim;

    /* create notes object collection */
    memset(&_jb_dim, 0, sizeof(struct _jb_dims));

    _obj = (struct _zobject*) malloc(sizeof(struct _zobject));
    _obj->type = zobject_item;

    _child = xmlFirstElementChild(node);
    _create_jb_object_helper(_child, (void*) &_jb_dim);

    /* create junction box object */
    _obj->data._i = zjb_new(NULL,
			    &parser->device,
			    _jb_dim.x,
			    _jb_dim.y,
			    _jb_dim.width,
			    _jb_dim.height,
			    _jb_dim.depth,
			    _jb_dim.angle);

    zjb_set_fillet_radius(Z_JB(_obj->data._i), _jb_dim.radius);

    while(_child)
	{
	    if(strcmp((const char*) _child->name, ZPARSER_TERMINALS) == 0)
		_create_jb_object_terminals_helper(_child, _obj->data._i);
	    else if(strcmp((const char*) _child->name, ZPARSER_GLAND) == 0)
		_create_jb_object_gland_helper(_child, _obj->data._i);

	    _child = xmlNextElementSibling(_child);
	}

    /* call draw method */
    zgeneric_draw(_obj->data._i);

    /* push to the collection */
    blist_add_from_end(&parser->object_array, (void*) _obj);

    return ZELIA_OK;
}

/* create leader object */
int _create_leader_object(xmlNodePtr node, struct _zparser* parser)
{
    xmlChar* _content = NULL;
    xmlNodePtr _child = NULL;
    double _dim_dbl = 0.0, _x = 0.0, _y = 0.0, _ang = 0.0, _seg1 = 0.0, _seg2 = 0.0;
    struct _zobject* _obj = NULL;
    unsigned int _del_flg = 1;

    _obj = (struct _zobject*) malloc(sizeof(struct _zobject));
    _obj->type = zobject_item;

    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    _content = xmlNodeGetContent(_child);
	    if(_content != NULL)
		_dim_dbl = atof((char*) _content);
	    
	    _del_flg = 1;
	    
	    if(strcmp((const char*) _child->name, ZPARSER_COORD_X) == 0)
		_x = _dim_dbl;
	    else if(strcmp((const char*) _child->name, ZPARSER_COORD_Y) == 0)
		_y = _dim_dbl;
	    else if(strcmp((const char*) _child->name, ZPARSER_ANGLE) == 0)
		_ang = _dim_dbl;
	    else if(strcmp((const char*) _child->name, ZPARSER_SEGMENT_1) == 0)
		_seg1 = _dim_dbl;
	    else if(strcmp((const char*) _child->name, ZPARSER_SEGMENT_2) == 0)
		_seg2 = _dim_dbl;
	    else if(strcmp((const char*) _child->name, ZPARSER_DESCRIPTION) == 0)
		_del_flg = 0;
	    
	    if(_del_flg)
		{
		    xmlFree(_content);
		    _content = NULL;
		}

	    _child = xmlNextElementSibling(_child);	    
	}

    /* create a new leader object */
    _obj->data._i = zleader_new(NULL,
				&parser->device,
				_x,
				_y,
				_seg1,
				_seg2,
				_ang,
				(char*) _content);
    /* call draw method */
    zgeneric_draw(_obj->data._i);

    /* push to the collection */
    blist_add_from_end(&parser->object_array, (void*) _obj);

    if(_content)
	xmlFree(_content);
    
    return ZELIA_OK;
}

int _create_label_object(xmlNodePtr node, struct _zparser* parser)
{
    struct _zobject* _obj = NULL;

    xmlNodePtr _child = NULL;
    xmlChar* _content = NULL;
    xmlChar* _path = NULL;
    unsigned int _del_flg = 1;
    double _x = 0.0, _y = 0.0;
    
    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    _del_flg = 1;
	    _content = xmlNodeGetContent(_child);
	    if(strcmp((const char*) _child->name, ZPARSER_TEMPLATE_PATH) == 0)
		{
		    _del_flg = 0;
		    _path = _content;
		}
	    else if(strcmp((const char*) _child->name, ZPARSER_COORD_X) == 0)
		_x = atof((const char*) _content);
	    else if(strcmp((const char*) _child->name, ZPARSER_COORD_Y) == 0)
		_y = atof((const char*) _content);
	    
	    /* if delete flag is true delete and set the pointer to NULL */
	    if(_del_flg)
		{
		    xmlFree(_content);
		    _content = NULL;
		}
	    
	    _child = xmlNextElementSibling(_child);
	}

    if(_path == NULL)
	return ZELIA_NULL;

    /* create object */
    _obj = (struct _zobject*) malloc(sizeof(struct _zobject));
    _obj->type = zobject_item;

    _obj->data._i = zlabel_new(NULL, &parser->file, (const char*) _path, _x, _y);
    
    /* call draw method */
    zgeneric_draw(_obj->data._i);

    /* push to the collection */
    blist_add_from_end(&parser->object_array, (void*) _obj);
    
    return ZELIA_OK;
}

/* delete objects by calling their destructor */
int _finalise_parser(struct _zparser* parser)
{
    const char* _buff = NULL;

    /* get buffer */
    _buff = zdevice_get_temp_buff(&parser->device);

    /* add to the file object */
    zfile_parse_and_insert_elements_as_new(&parser->file, _buff);

    /* clean up */
    blist_delete(&parser->object_array);
    zdevice_delete(&parser->device);

    zfile_attrib_delete(&parser->attrib);
    zfile_delete(&parser->file);

    return ZELIA_OK;
}

/* delete helper for clearing the object array */
static void _delete_helper(void* data)
{
    struct _zobject* _obj = NULL;

    if(data == NULL)
	return;

    _obj = (struct _zobject*) data;

    if(_obj->type == zobject_item)
	{
	    zgeneric_delete(_obj->data._i);
	    _obj->data._i = NULL;
	}
    else if(_obj->type == zobject_cols)
	{
	    zgenerics_delete(_obj->data._c);
	    _obj->data._c = NULL;
	}

    free(data);
    return;
}

/* helper method for setting the width of columns */
inline __attribute__ ((always_inline)) static int _create_table_object_dim(xmlNodePtr node, zgeneric* object)
{
    int _ix = 0;
    double _w = 0.0;
    xmlChar* _val = NULL;
    xmlChar* _attrib_val = NULL;

    /* check if width property was set, if found set the value */
    _attrib_val = xmlGetProp(node, (xmlChar*) ZPARSER_COLUMN_WIDTH_ATTRIB);
    _val = xmlNodeGetContent(node);
    if(_attrib_val != NULL)
	_w = atof((char*) _attrib_val);

    if(_val != NULL)
	_ix = atoi((char*) _val);

    ztable_set_column_width(Z_TABLE(object), _ix, _w);

    xmlFree(_attrib_val);
    xmlFree(_val);

    return ZELIA_OK;
}

/* set table content */
inline __attribute__ ((always_inline)) static int _create_table_object_content(xmlNodePtr node, const xmlChar* content, zgeneric* object)
{
    int _col_ix = 0, _row_ix = 0;
    xmlChar* _col_ch = NULL, *_row_ch = NULL;

    _col_ch = xmlGetProp(node, (xmlChar*) ZPARSER_COL_IX_ATTRIB);
    _row_ch = xmlGetProp(node, (xmlChar*) ZPARSER_ROW_IX_ATTRIB);

    if(_col_ch)
	{
	    _col_ix = atoi((char*) _col_ch);
	    xmlFree(_col_ch);
	}

    if(_row_ch)
	{
	    _row_ix = atoi((char*) _row_ch);
	    xmlFree(_row_ch);
	}

    /* if content pointer is not null we set the content */
    ztable_set_content(Z_TABLE(object), _row_ix, _col_ix, (const char*) content);
    return ZELIA_OK;
}


inline __attribute__ ((always_inline)) static int _create_jb_object_helper(xmlNodePtr node, void* object)
{
    xmlChar *_xs = NULL;
    xmlChar *_ys = NULL;
    xmlChar *_widths = NULL;
    xmlChar *_heights = NULL;
    xmlChar *_depths = NULL;
    xmlChar *_angles = NULL;
    xmlChar *_radiuss = NULL;
    double _x = 0.0, _y = 0.0, _width = 0.0, _height = 0.0, _depth = 0.0, _angle = 0.0, _radius = 0.0;
    double* _ptr = (double*) object;

    xmlNodePtr _child = NULL;

    _child = node;
    while(_child)
	{

	    if(strcmp((const char*) _child->name, ZPARSER_COORD_X) == 0)
		_xs = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_COORD_Y) == 0)
		_ys = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_COLUMN_WIDTH_ATTRIB) == 0)
		_widths = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_ROW_HEIGHT_ATTRIB) == 0)
		_heights = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_DEPTH) == 0)
		_depths = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_RADIUS) == 0)
		_radiuss = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_ANGLE) == 0)
		_angles = xmlNodeGetContent(_child);

	    /* if all variables are set we exit loop here, otherwise continue looking */
	    if(_xs && _ys && _widths && _heights && _depths && _radiuss && _angles)
		break;

    	    _child = xmlNextElementSibling(_child);
	}


    /* convert the variables and free the buffers */
    if(_xs)
	{
	    _x = atof((const char*) _xs);
	    xmlFree(_xs);
	}
    if(_ys)
	{
	    _y = atof((const char*) _ys);
	    xmlFree(_ys);
	}

    if(_widths)
	{
	    _width = atof((const char*) _widths);
	    xmlFree(_widths);
	}

    if(_heights)
	{
	    _height = atof((const char*) _heights);
	    xmlFree(_heights);
	}

    if(_depths)
	{
	    _depth = atof((const char*) _depths);
	    xmlFree(_depths);
	}

    if(_radiuss)
	{
	    _radius = atof((const char*) _radiuss);
	    xmlFree(_radiuss);
	}

    if(_angles)
	{
	    _angle = atof((const char*) _angles);
	    xmlFree(_angles);
	}

    *_ptr = _x;
    _ptr++;

    *_ptr = _y;
    _ptr++;

    *_ptr = _width;
    _ptr++;

    *_ptr = _height;
    _ptr++;

    *_ptr = _depth;
    _ptr++;

    *_ptr = _radius;
    _ptr++;

    *_ptr = _angle;

    return ZELIA_OK;
}

inline __attribute__ ((always_inline)) static int _create_jb_object_terminals_helper(xmlNodePtr node, zgeneric* object)
{
    xmlChar* _nums = NULL, *_widths = NULL, *_heights = NULL, *_links = NULL;
    int _num = 0;
    double _width = 0.0, _height = 0.0;

    xmlNodePtr _child = NULL;

    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    if(strcmp((const char*) _child->name, ZPARSER_TERMINALS_NUM) == 0)
		_nums = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_COLUMN_WIDTH_ATTRIB) == 0)
		_widths = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_ROW_HEIGHT_ATTRIB) == 0)
		_heights = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_TERMINALS_LINKS) == 0)
		_links = xmlNodeGetContent(_child);

	    /* break from loop if we have obtained every thing */
	    if(_nums && _widths && _heights && _links)
		break;
    	    _child = xmlNextElementSibling(_child);
	}


    if(_nums)
	{
	    _num = atoi((const char*) _nums);
	    xmlFree(_nums);
	}

    if(_widths)
	{
	    _width = atof((const char*) _widths);
	    xmlFree(_widths);
	}

    if(_heights)
	{
	    _height = atof((const char*) _heights);
	    xmlFree(_heights);
	}

    /* check links pointer and add terminals */
    if(_links)
	{
	    zjb_add_terminals(Z_JB(object),
			      _num,
			      _width,
			      _height,
			      (const char*) _links);
	    xmlFree(_links);
	}
    else
	{
	    zjb_add_terminals(Z_JB(object),
			      _num,
			      _width,
			      _height,
			      NULL);
	}

    return ZELIA_OK;
}


inline __attribute__ ((always_inline)) static int _create_jb_object_gland_helper(xmlNodePtr node, zgeneric* object)
{
    xmlChar *_xs = NULL;
    xmlChar *_ys = NULL;
    xmlChar *_szs = NULL;
    xmlChar *_hexs = NULL;

    double _x = 0.0, _y = 0.0;
    zGlandSize _sz = zM16;
    int _hex = 0;
    xmlNodePtr _child = NULL;

    _child = xmlFirstElementChild(node);
    while(_child)
	{
	    if(strcmp((const char*) _child->name, ZPARSER_COORD_X) == 0)
		_xs = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_COORD_Y) == 0)
		_ys = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_SIZE) == 0)
		_szs = xmlNodeGetContent(_child);
	    else if(strcmp((const char*) _child->name, ZPARSER_HEX_FLG) == 0)
		_hexs = xmlNodeGetContent(_child);

	    /* break if all interested variables are set */
	    if(_xs && _ys && _szs && _hexs)
		break;

    	    _child = xmlNextElementSibling(_child);
	}


    if(_xs)
	{
	    _x = atof((const char*) _xs);
	    xmlFree(_xs);
	}

    if(_ys)
	{
	    _y = atof((const char*) _ys);
	    xmlFree(_ys);
	}

    if(_szs)
	{
	    if(strcmp((const char*) _szs, ZPARSER_GLAND_SZ_M16) == 0)
		_sz = zM16;
	    else if(strcmp((const char*) _szs, ZPARSER_GLAND_SZ_M20) == 0)
		_sz = zM20;
	    else
		_sz = zM25;

	    xmlFree(_szs);
	}

    if(_hexs)
	{
	    _hex = atoi((const char*) _hexs);
	    xmlFree(_hexs);
	}

    /* add glands */
    zjb_add_glands(Z_JB(object), _x, _y, _sz, _hex);

    return ZELIA_OK;
}
