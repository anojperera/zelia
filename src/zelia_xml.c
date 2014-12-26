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

struct _zobject
{
    enum _zobject_primary_type
	{
	    zobject_item,
	    zobject_cols
	} type;

    union _zobject_data
    {
	zgeneric* _i;
	zgenerics* _c;
    } data;
};

struct _zparser
{
    zfile file;
    zfile_attrib attrib;
    zdevice device;
    blist object_array;
};

static int _read_file(const char* temp_path, char** buff, size_t* size);
static zelia_object_types _get_type(const char* tag_name);
int _main_loop(xmlNodePtr node, struct _zparser* parser);

static void _delete_helper(void* data);

int _create_file_object(xmlNodePtr node, struct _zparser* parser);
int _create_attrib_object(xmlNodePtr node, struct _zparser* parser);
int _create_notes_object(xmlNodePtr node, struct _zparser* parser);
int _create_table_object(xmlNodePtr node, struct _zparser* parser);

inline __attribute__ ((always_inline)) static int _create_table_object_dim(xmlNodePtr node, zgeneric* object);
inline __attribute__ ((always_inline)) static int _create_table_object_content(xmlNodePtr node, const xmlChar* content, zgeneric* object);
    
int _finalise_parser(struct _zparser* parser);

int zelia_parse_file(const char* xml_path);
/* int zelia_parse_buffer(const char* buff, const size_t sz); */

/* parse xml file and create objects */
int zelia_parse_file(const char* xml_path)
{
    char* _buff = NULL;					/* buffer to hold the file contents */
    size_t _sz = 0;					/* buffer file size */

    xmlDocPtr _xml_doc = NULL;				/* xml document pointer */
    xmlNodePtr _root = NULL;

    struct _zparser _parser;

    /* check arguments */
    ZCHECK_OBJ_INT(xml_path);

    /* read the file */
    if(_read_file(xml_path, &_buff, &_sz) != ZELIA_OK)
	return ZELIA_PARSER_ERROR;

    memset((void*) &_parser, 0, sizeof(struct _zparser));

    /* initialise object array */
    blist_new(&_parser.object_array, _delete_helper);

    if(!zfile_new(&_parser.file))
	return ZELIA_PARSER_ERROR;

    /* toggle the force overwrite mode */
    zfile_toggle_overwrite(&_parser.file);


    /* parse the document */
    xmlInitParser();

    _xml_doc = xmlParseMemory(_buff, _sz);

    if(_xml_doc == NULL)
	{
	    ZELIA_LOG_MESSAGE("zelia_xml unable to parse the document");
	    if(_buff != NULL && _sz > 0)
		free(_buff);
	    _buff = NULL;
	    _sz = 0;
	    return ZELIA_PARSER_ERROR;
	}

    _root = xmlDocGetRootElement(_xml_doc);

    _main_loop(xmlFirstElementChild(_root), &_parser);

    _finalise_parser(&_parser);
    xmlFreeDoc(_xml_doc);
    xmlCleanupParser();

    if(_buff != NULL && _sz > 0)
	free(_buff);
    _buff = NULL;
    _sz = 0;

    return ZELIA_OK;
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

/* delete objects by calling their destructor */
int _finalise_parser(struct _zparser* parser)
{
    const char* _buff = NULL;

    /* get buffer */
    _buff = zdevice_get_temp_buff(&parser->device);

    /* add to the file object */
    zfile_parse_and_insert_elements(&parser->file, _buff);

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
