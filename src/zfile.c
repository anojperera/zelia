/* implementation of file handling class */
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libxml/tree.h>

#include "zVar.h"
#include "zfile.h"

char* _zfile_read_contents(const char* file_path, char** buff, size_t* sz);

static int _zfile_parse_xml(zfile* obj);
static int _zfile_callback_hook(void* obj);
static int _zfile_find_node_by_name(xmlDocPtr doc, const char* name, xmlNodePtr* node);

#define ZFILE_DEF_NAME "defs"
#define ZFILE_G_NAME "g"
#define ZFILE_SVG_NAME "svg"

#define ZFILE_PARSED_SVG_SECTIONS 2
#define ZFILE_TRANSFORM_BUFF 512
#define ZFILE_TRANSFORM_ATTRIB "transform"
#define ZFILE_TRANSLATE_ATTRIB "translate (%f, %f)"

/* constructor */
zfile* zfile_new(zfile* obj)
{
    if(obj == NULL)
	{
	    obj = (zfile*) malloc(sizeof(zfile));
	    obj->_init_flg = ZELIA_CONSTRUCTED;
	    ZELIA_LOG_MESSAGE("zfile object created");
	}
    else
	{
	    obj->_init_flg = ZELIA_INITIALISED;
	    ZELIA_LOG_MESSAGE("zfile object is initialised");
	}

    obj->force_flg = ZELIA_FILE_FORCE_DEFAULT;

    obj->tmp_file = NULL;
    memset((void*) obj->new_file_path, 0, Z_FILE_PATH_SZ);
    memset((void*) obj->new_file_name, 0, Z_FILE_NAME_SZ);

    obj->read_buff = NULL;
    obj->read_buff_sz = 0;

    obj->callback_hook = _zfile_callback_hook;

    /* interanl string struct pointers for easy handling */
    obj->_string.buff = obj->read_buff;
    obj->_string.buff_sz_ptr = &obj->read_buff_sz;

    obj->xml_doc = NULL;
    obj->ext_ptr = NULL;
    obj->child = NULL;

    return obj;
}

/* destructor */
void zfile_delete(zfile* obj)
{
    if(obj == NULL)
	return;

    obj->tmp_file = NULL;

    if(obj->read_buff != NULL && obj->read_buff_sz > 0)
	free(obj->read_buff);

    obj->read_buff = NULL;
    obj->read_buff_sz = 0;

    obj->callback_hook = NULL;

    /* set struct to NULL */
    obj->_string.buff = NULL;
    obj->_string.buff_sz_ptr = NULL;

    if(obj->xml_doc)
	{
	    xmlFreeDoc(obj->xml_doc);
	    xmlCleanupParser();
	}

    obj->child = NULL;
    obj->ext_ptr = NULL;

    /* if created destroy it */
    if(obj->_init_flg == ZELIA_CONSTRUCTED)
	free(obj);

    ZELIA_LOG_MESSAGE("zfile object deleted successfully");
    return;
}

/* create a file from a template */
int zfile_create_file_from_template(zfile* obj, const char* tmp_file, const char* file_path)
{
    int r_fd = 0, w_fd = 0;								/* read and write file descriptors */
    struct stat tmp_stat;								/* struct for holding the template file stat */
    mode_t prev_mode, current_mode;							/* previous file mode */

    if(obj == NULL || tmp_file == NULL || file_path == NULL)
	return ZELIA_NULL;

    /* set the template file path */
    obj->tmp_file = tmp_file;

    ZELIA_LOG_MESSAGE("zfile object checking if file exists");
    /*
     * Copy the file path to internal if the file does not exists.
     * If it exists, we check for force flag and copy to internal.
     */
    if(access(file_path, F_OK) || (!access(file_path, W_OK) && obj->force_flg == ZELIA_FORCE_OVERWRITE))
	{
	    /* file doesn't exist, therefore we copy to internal */
	    strncpy(obj->new_file_path, file_path, Z_FILE_PATH_SZ-1);
	    obj->new_file_path[Z_FILE_PATH_SZ-1] = '\0';
	    ZELIA_LOG_MESSAGE("zfile file path copied to internal successfully");
	}
    else
	{
	    ZELIA_LOG_MESSAGE("zfile file path copy error, bailing out");
	    return ZELIA_FILE_COPY_ERROR;
	}

    /* check to see if the temporary file exist */
    ZELIA_LOG_MESSAGE("zfile, checking if template file exists");
    if(access(obj->tmp_file, F_OK))
	{
	    ZELIA_LOG_MESSAGE("zfile template does not exist, bailing out");
	    return ZELIA_FILE_COPY_ERROR;
	}

    /* reading file and copy to new location */
    ZELIA_LOG_MESSAGE("zfile opening template");

    r_fd = open(obj->tmp_file, O_RDONLY);
    if(r_fd == -1)
	{
	    ZELIA_LOG_MESSAGE("zfile errors occured while reading the template");

	    return ZELIA_FILE_COPY_ERROR;
	}

    /* set the umask mode for the file creation */
    current_mode = S_IXOTH | S_IWOTH | S_IWGRP | S_IXGRP | S_IXUSR;
    prev_mode = umask(current_mode);
    w_fd = open(obj->new_file_path, O_RDWR | O_CREAT);

    /* once open reset the file mask */
    umask(prev_mode);

    if(w_fd == -1)
	{
	    /* write file descriptor failed, so we bail */
	    ZELIA_LOG_MESSAGE("zfile write file descriptor failed");
	    ZELIA_LOG_MESSAGE("zfile closing template file");

	    /* close template file */
	    close(r_fd);

	    return ZELIA_FILE_COPY_ERROR;
	}

    fchmod(w_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    /* check stat for file size */
    if(fstat(r_fd, &tmp_stat))
	{
	    ZELIA_LOG_MESSAGE("zfile unable to get tempalate file size");
	    ZELIA_LOG_MESSAGE("zfile unable closing template file and write file");

	    close(r_fd);
	    close(w_fd);

	    /* delete the new file */
	    unlink(obj->new_file_path);
	    ZELIA_LOG_MESSAGE("zfile created file deleted due to errors");
	    return ZELIA_FILE_COPY_ERROR;
	}


    /* read contents of the the tempalate file */
    /* create buffer */
    obj->read_buff_sz = tmp_stat.st_size + 1;
    obj->read_buff = (char*) malloc((size_t) obj->read_buff_sz);

    if(!read(r_fd, (void*) obj->read_buff, tmp_stat.st_size))
	{
	    ZELIA_LOG_MESSAGE("zfile template file read error, bailing out");

	    close(r_fd);
	    close(w_fd);

	    unlink(obj->new_file_path);
	    ZELIA_LOG_MESSAGE("zfile created file deleted due to errors");

	    return ZELIA_FILE_COPY_ERROR;
	}

    /* write to disc */
    if(!write(w_fd, obj->read_buff, tmp_stat.st_size))
	{
	    ZELIA_LOG_MESSAGE("zelia copying failed");

	    close(r_fd);
	    close(w_fd);

	    unlink(obj->new_file_path);
	    ZELIA_LOG_MESSAGE("zfile created file deleted due to errors");

	    return ZELIA_FILE_COPY_ERROR;
	}

    fsync(w_fd);

    /* close all file descriptors and exit */
    close(r_fd);
    close(w_fd);

    /* call to parse the xml document */
    _zfile_parse_xml(obj);

    return ZELIA_OK;
}

int zfile_update_file(zfile* obj)
{
    if(obj == NULL)
	return ZELIA_NULL;

    if(obj->xml_doc == NULL)
	return ZELIA_NULL;


    /* check if the file exists */
    if(access(obj->new_file_path, F_OK))
	{
	    ZELIA_LOG_MESSAGE("file does not exist");
	    return ZELIA_NULL;
	}

    if(xmlSaveFile(obj->new_file_path, obj->xml_doc) != -1)
	{
	    ZELIA_LOG_MESSAGE("file written successfully");
	    /* update cache here */
	}

    return ZELIA_OK;
}

int zfile_update_cache(zfile* obj)
{
    struct stat _stat;
    int r_fd = 0;
    char* read_buff = NULL, *old_buff = NULL;
    ssize_t read_buff_sz = 0;

    if(obj == NULL)
	return ZELIA_NULL;

    /* check if file paths are set */
    if(obj->new_file_path[0] == 0)
	{
	    ZELIA_LOG_MESSAGE("zfile cache update error, file path was not set");
	    return ZELIA_CACHE_ERROR;
	}

    /* check if file exists */
    if(access(obj->new_file_path, F_OK))
	{
	    ZELIA_LOG_MESSAGE("zfile file not found");
	    return ZELIA_CACHE_ERROR;
	}

    /* open file and read the contents */
    r_fd = open(obj->new_file_path, O_RDONLY);
    if(r_fd == -1)
	{
	    ZELIA_LOG_MESSAGE("zfile unable to open the file");
	    return ZELIA_CACHE_ERROR;
	}

    /* get stat */
    if(fstat(r_fd, &_stat))
	{
	    ZELIA_LOG_MESSAGE("zfile unable to get the stats for the file");
	    ZELIA_LOG_MESSAGE("zfile closing file descriptor");
	    return ZELIA_CACHE_ERROR;
	}

    read_buff_sz = _stat.st_size+1;
    read_buff = (char*) malloc(read_buff_sz);

    if(!read(r_fd, read_buff, _stat.st_size))
	{
	    ZELIA_LOG_MESSAGE("unable to read the file, bailing out");
	    ZELIA_LOG_MESSAGE("zfile closing file descriptor");

	    return ZELIA_CACHE_ERROR;
	}

    /* destroy the old buffer and use the new one */
    old_buff = obj->read_buff;

    obj->read_buff = read_buff;
    obj->read_buff_sz = (size_t) read_buff_sz;

    /* remove old buffer */
    free(old_buff);

    return ZELIA_OK;
}

/* parse elements */
int zfile_parse_and_insert_elements(zfile* obj, const char* buff)
{
    xmlDocPtr _p_doc = NULL;							/* document pointer of parsed buffer */
    xmlNodePtr _m_itr = NULL;
    xmlNodePtr _p_def_itr = NULL;						/* parsed document iterator */
    xmlNodePtr _p_g_itr = NULL;							/* iterator for elements */

    xmlNodePtr _p_def_copy = NULL;						/* copy of def object */
    xmlNodePtr _p_g_copy = NULL;						/* copy of g object */
    xmlNodePtr _itr = NULL;

    /* check arguments */
    ZCHECK_OBJ_INT(obj);
    ZCHECK_OBJ_INT(buff);

    /* parse buffer */
    ZELIA_LOG_MESSAGE("zfile begin parsing buffer");

    /* parse document */
    _p_doc = xmlParseDoc((const xmlChar*) buff);
    if(_p_doc == NULL)
	return ZELIA_FILE_COPY_ERROR;

    /* check if we find the correct node */
    if(_zfile_find_node_by_name(obj->xml_doc, ZFILE_DEF_NAME, &_m_itr) != ZELIA_OK)
	{
	    ZELIA_LOG_MESSAGE("zfile failed to find node while in main document");
	    xmlFreeDoc(_p_doc);
	    return ZELIA_FILE_COPY_ERROR;
	}


    if(_zfile_find_node_by_name(_p_doc, ZFILE_DEF_NAME, &_p_def_itr) != ZELIA_OK)
	ZELIA_LOG_MESSAGE("zfile failed to find node while in parsed document");


    /* get first element */
    _itr = xmlFirstElementChild(_p_def_itr);
    while(_itr) {

	/* copy node */
	_p_def_copy = xmlCopyNode(_itr, 1);

	if(xmlAddChild(_m_itr, _p_def_copy) == NULL)
	    {
		ZELIA_LOG_MESSAGE("zfile copying of definitions element to the main document failed");
		/* xmlFreeDoc(_p_doc); */
		return ZELIA_FILE_COPY_ERROR;
	    }

	_itr = xmlNextElementSibling(_itr);
    }


    /* check if we find the correct node */
    if(_zfile_find_node_by_name(obj->xml_doc, ZFILE_SVG_NAME, &_m_itr) != ZELIA_OK)
	{
	    ZELIA_LOG_MESSAGE("zfile failed to find node while in main document");
	    /* xmlFreeDoc(_p_doc); */
	    return ZELIA_FILE_COPY_ERROR;
	}

    if(_zfile_find_node_by_name(_p_doc, ZFILE_SVG_NAME, &_p_g_itr) != ZELIA_OK)
	{
	    ZELIA_LOG_MESSAGE("zfile failed to find node while in main document");
	    xmlFreeDoc(_p_doc);
	    return ZELIA_FILE_COPY_ERROR;
	}


    _itr = xmlFirstElementChild(_p_g_itr);
    while(_itr) {
	/* copy element */
	_p_g_copy = xmlCopyNode(_itr, 1);

	if(xmlAddChild(_m_itr, _p_g_copy) == NULL)
	    {
		ZELIA_LOG_MESSAGE("zfile copying of drawing elements to the main document failed");
		/* xmlFreeDoc(_p_doc); */
		return ZELIA_FILE_COPY_ERROR;
	    }

	_itr = xmlNextElementSibling(_itr);
    }

    /* call to update the buffer */
    zfile_update_file(obj);
    zfile_update_cache(obj);

    /* free document */
    xmlFreeDoc(_p_doc);
    return ZELIA_OK;
}

int zfile_parse_and_insert_elements_as_new_with_coords(zfile* obj,
						       const char* buff,
						       const double* x,
						       const double* y)
{
    double _x = 0.0, _y = 0.0;
    int i = 0;
    xmlDocPtr _p_doc = NULL;							/* document pointer of parsed buffer */
    xmlNodePtr _m_itr = NULL;							/* main xml iterator of document */
    char _buff[ZFILE_TRANSFORM_BUFF];

    /*
     * Array of xml nodes for finding the definitions and elements section
     * of the parsed xml document.
     */
    xmlNodePtr _p_itr[ZFILE_PARSED_SVG_SECTIONS] = {NULL, NULL};
    xmlNodePtr _n_ptr = NULL;							/* new xml node */
    xmlNodePtr _p_itr_copy = NULL;
        
    /* check arguments */
    ZCHECK_OBJ_INT(obj);
    ZCHECK_OBJ_INT(buff);

    memset(_buff, 0, ZFILE_TRANSFORM_BUFF);
    
    /* parse buffer */
    ZELIA_LOG_MESSAGE("zfile begin parsing buffer");

    /* parse document */
    _p_doc = xmlParseDoc((const xmlChar*) buff);
    if(_p_doc == NULL)
	return ZELIA_FILE_COPY_ERROR;

    /* find definitions section */
    if(_zfile_find_node_by_name(_p_doc, ZFILE_DEF_NAME, &_p_itr[0]) != ZELIA_OK)
	ZELIA_LOG_MESSAGE("zfile failed to find node while in parsed document");

    /* find elements section */
    if(_zfile_find_node_by_name(_p_doc, ZFILE_G_NAME, &_p_itr[1]) != ZELIA_OK)
	{
	    ZELIA_LOG_MESSAGE("zfile failed to find node while in main document");
	    xmlFreeDoc(_p_doc);
	    return ZELIA_FILE_COPY_ERROR;
	}

    /* find the main svg item in document */
    if(_zfile_find_node_by_name(obj->xml_doc, ZFILE_SVG_NAME, &_m_itr) != ZELIA_OK)
	{
	    ZELIA_LOG_MESSAGE("zfile failed to find node while in main document");
	    xmlFreeDoc(_p_doc);
	    return ZELIA_FILE_COPY_ERROR;
	}

    /* create new element under ZFILE_SVG_NAME */
    _n_ptr = xmlNewChild(_m_itr, NULL, (const xmlChar*) ZFILE_G_NAME, NULL);
    for(i = 0; i < ZFILE_PARSED_SVG_SECTIONS; i++)
	{
	    if(_p_itr[i] == NULL)
		continue;

	    /* copy and add to the element */
	    _p_itr_copy = xmlCopyNode(_p_itr[i], 1);
	    
	    if(xmlAddChild(_n_ptr, _p_itr_copy) == NULL)
		ZELIA_LOG_MESSAGE("errors occured during adding new element to the xmldoc");
	}

    /* add attribute */
    if(_n_ptr && x && y)
	{
	    _x = ZCONV_TO_POINTS(*x);
	    _y = ZCONV_TO_POINTS(*y);
	    sprintf(_buff, ZFILE_TRANSLATE_ATTRIB, (float) _x, (float) _y);

	    xmlSetProp(_n_ptr, (const xmlChar*) ZFILE_TRANSFORM_ATTRIB, (const xmlChar*) _buff);
	}
    
    /* call to update the buffer */
    zfile_update_file(obj);
    zfile_update_cache(obj);

    /* free document */
    xmlFreeDoc(_p_doc);
    return ZELIA_OK;    
}
/*=================================== Private Methods ===================================*/

/* parse the xml document */
static int _zfile_parse_xml(zfile* obj)
{

    if(obj->read_buff == NULL || obj->read_buff_sz <= 0)
	return ZELIA_NULL;

    if(!obj->xml_doc)
	xmlInitParser();
    else
	{
	    ZELIA_LOG_MESSAGE("zfile xml_doc already created, freeing before using");
	    xmlFreeDoc(obj->xml_doc);
	}

    ZELIA_LOG_MESSAGE("parsing memory");
    obj->xml_doc = xmlParseMemory(obj->read_buff, obj->read_buff_sz);
    if(obj->xml_doc)
	ZELIA_LOG_MESSAGE("xml parsed successfully");
    else
	ZELIA_LOG_MESSAGE("xml parsing failed, document pointer set to NULL");

    return ZELIA_OK;
}

/* callback hook implementation */
static int _zfile_callback_hook(void* obj)
{
    zfile* _obj;

    if(obj == NULL)
	return ZELIA_NULL;

    /* cast object */
    _obj = (zfile*) obj;
    return zfile_update_file(_obj);
}

/* find element named */
static int _zfile_find_node_by_name(xmlDocPtr doc, const char* name, xmlNodePtr* node)
{
    xmlNodePtr _root = NULL;
    xmlNodePtr _child = NULL;
    unsigned int _found = 0;

    /* check for arguments */
    ZCHECK_OBJ_INT(doc);
    ZCHECK_OBJ_INT(name);
    ZCHECK_OBJ_INT(node);

    /* get root node */
    ZELIA_LOG_MESSAGE_WITH_STR("zfile finding node ", name);

    *node = NULL;
    _root = xmlDocGetRootElement(doc);
    while(_root)
	{
	    ZELIA_LOG_MESSAGE("zfile looking in main child");
	    if(strcmp((const char*) _root->name, name) == 0)
		{
		    ZELIA_LOG_MESSAGE("zfile element found exiting loop");
		    *node = _root;
		    break;
		}

	    /* get the child node */
	    ZELIA_LOG_MESSAGE("zfile looking in child node");
	    _child = xmlFirstElementChild(_root);
	    while(_child)
		{
		    if(strcmp((const char*) _child->name, name) == 0)
			{
			    ZELIA_LOG_MESSAGE("zfile element found in child exiting inner loop");
			    *node = _child;
			    _found = 1;
			    break;
			}
		    _child = xmlNextElementSibling(_child);
		}

	    /* if found break out of main loop */
	    if(_found > 0)
		break;

	    _root = xmlNextElementSibling(_root);
	}

    if(*node == NULL)
	return ZELIA_NULL;
    else
	return ZELIA_OK;
}

char* _zfile_read_contents(const char* file_path, char** buff, size_t* sz)
{
    int r_fd = 0;
    struct stat tmp_stat;								/* struct for holding the template file stat */
    ssize_t _rd_sz = 0;
    char* _buff_t = NULL;
    
    ZELIA_LOG_MESSAGE_WITH_STR("zfile check file exist ", file_path);
    if(access(file_path, F_OK))
	{
	    ZELIA_LOG_MESSAGE("zfile file does not exist");
	    return NULL;
	}

    /* open file */
    r_fd = open(file_path, O_RDONLY);
    if(r_fd == -1)
	{
   	    ZELIA_LOG_MESSAGE("zfile errors occured while reading the template");
	    return NULL;
	}

    /* check stats for the file to obtain the size */
    ZELIA_LOG_MESSAGE("checking file stats");
    if(fstat(r_fd, &tmp_stat))
	{
	    ZELIA_LOG_MESSAGE("zfile unable to get tempalate file size");
	    ZELIA_LOG_MESSAGE("zfile unable closing template file and write file");
	    close(r_fd);
	    return NULL;
	}

    *sz = tmp_stat.st_size+1;
    *buff = (char*) malloc(tmp_stat.st_size+1);
    _rd_sz = read(r_fd, (void*) *buff, tmp_stat.st_size);
    if(_rd_sz <= 0)
	{
	    ZELIA_LOG_MESSAGE("zfile unable to read the file");
	    free(*buff);
	    *buff = NULL;
	    *sz = 0;
	    close(r_fd);

	    return NULL;
	}
    
    /* NULL terminate buffer */
    _buff_t = *buff + _rd_sz;
    *_buff_t = '\0';
    
    /* close the file descriptor */
    close(r_fd);
    ZELIA_LOG_MESSAGE("zfile file read and loaded in to temporary buffer");
    
    return *buff;
}
