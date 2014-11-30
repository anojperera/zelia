/*
 * class for handling file attributes.
 */

#ifndef _ZFILE_ATTRIB_H_
#define _ZFILE_ATTRIB_H_

#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>


#define ZFILE_ATTRIB_FIELD_SZ 128

/* typedef class and field type */
typedef struct _zfile_attrib zfile_attrib;

struct zfile_attrib_field
{
    char* field_id;
    char* field_value;
};

struct _zfile_attrib
{
    unsigned int _init_flg;						/* internal flag */
    unsigned int field_cr_flg;						/* flag to indicate field array was created */
    
    size_t field_count;							/* number of fields */
    const char* attrib_tbl_name;					/* attribute table name */
    
    struct zfile_attrib_field* field_array;				/* field array */
    
    xmlDocPtr xml_doc;							/* xml document pointer */

    int (*update_complete_callback)(void*);				/* callback hook */
    void* ext_obj;							/* external object pointer */
    void* child;							/* child pointer */
};

#ifdef __cpluplus
extern "C" {
#endif

    /* constructor and destructor */
    zfile_attrib* zfile_attrib_new(zfile_attrib* obj, xmlDocPtr doc);
    void zfile_attrib_delete(zfile_attrib* obj);

    /*
     * set the field array, if size was greater than zero, it shall be used, if 0 was set,
     * the method shall determine the size of the array.
     */
    int zfile_attrib_set_field_array(zfile_attrib* obj, struct zfile_attrib_field* array, size_t sz);

    /*
     * This method shall open the document pointed by file_path and
     * parse the xml and generate internal struct array.
     * If this method was called before zfile_attrib_set_field_array(), can pass NULL to second argument
     * of zfile_attrib_set_field_array()
     */
    int zfile_attrib_parse_attrib_xml(zfile_attrib* obj, const char* file_path);
    
#define zfile_attrib_set_table_name(obj, value)	\
    (obj)->attrib_tbl_name = (value)
    
    inline __attribute__ ((always_inline)) static int zfile_attrib_set_ext_obj(zfile_attrib* obj, void* ext_obj)
    {
	if(obj == NULL)
	    return ZELIA_NULL;

	obj->ext_obj = ext_obj;
	return ZELIA_OK;
    }
#define zfile_attrib_add_complete_hook(obj, hook)	\
    (obj)->update_complete_callback = (hook)
    
#ifdef __cplusplus
}
#endif

#endif /* _ZFILE_ATTRIB_H_ */
