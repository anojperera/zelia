/*
 * All file related operations are handled by this
 * class.
 * Sat Nov  8 17:29:05 GMT 2014
 */
#ifndef _ZFILE_H_
#define _ZFILE_H_

#include <stdlib.h>
#include <libxml/parser.h>


typedef struct _zfile zfile;

struct zfile_string
{
    const char* buff;
    const size_t* buff_sz_ptr;
};

struct _zfile
{
    unsigned int _init_flg;						/* internal flag */
    unsigned int force_flg;						/* force to overwrite the file */
    const char* tmp_file;						/* template path */
    char new_file_path[Z_FILE_PATH_SZ];					/* new file path */
    char new_file_name[Z_FILE_NAME_SZ];					/* new file directory */

    char* read_buff;							/* read buffer */
    size_t read_buff_sz;						/* read buffer size */

    int (*callback_hook)(void*);					/* callback hook */
    struct zfile_string _string;					/* internal string handling struct */
    xmlDocPtr xml_doc;							/* xml document pointer */
    void* ext_ptr;							/* external pointer */
    void* child;							/* child pointer for any derrived classes */
};

#ifdef __cpluplus
extern "C" {
#endif

    /* constructor and destructor */
    zfile* zfile_new(zfile* obj);
    void zfile_delete(zfile* obj);

    int zfile_create_file_from_template(zfile* obj, const char* tmp_file, const char* file_path);

    /* updates the internal cache */
    int zfile_update_file(zfile* obj);
    int zfile_update_cache(zfile* obj);

    /*
     * Parse elements and add to the file.
     * This method parses NULL terminated buffer pointed by buff
     */
    int zfile_parse_and_insert_elements(zfile* obj, const char* buff);
    
    /* property macros */
#define zfile_toggle_overwrite(obj)			\
    if((obj)->force_flg == ZELIA_FORCE_OVERWRITE)	\
	(obj)->force_flg = ZELIA_FILE_FORCE_DEFAULT;	\
    else						\
	(obj)->force_flg = ZELIA_FORCE_OVERWRITE
    
#define zfile_get_xmldoc_ptr(obj)		\
    (obj)->xml_doc

#define zfile_get_callback_hook(obj)		\
    (obj)->callback_hook
    /*
     * Get a pointer to a struct containing information of internal buffer.
     * This buffer maintains a cache of the read new file.
     */
    inline __attribute__ ((always_inline)) static const struct zfile_string* zfile_get_buffer(const zfile* obj)
    {
	if(obj == NULL)
	    return NULL;

	return &obj->_string;
    }
    
#ifdef __cplusplus
}
#endif

#endif /* _ZFILE_H_ */

