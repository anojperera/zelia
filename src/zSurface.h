/*
 * Primary surface object. This could be svg, pdf, or png.
 * Contains file information etc
 * Sun Jul 27 00:49:44 BST 2014
 */

#ifndef _ZSURFACE_H_
#define _ZSURFACE_H_


#include <cario/cario_surface.h>
#include "zVar.h"

#include <libxml/tree.h>

/* typedef the underlying struct */
typedef struct _zSurface zSurface;

struct _zSurface
{
    unsigned int var_init_flg;							/* flag to indicate initialised */
    unsigned int var_err_flg;							/* flag to indicate errors occured */
    char var_file_name[Z_FILE_NAME_SZ];						/* file name buffer */

    zFile_Type var_file_type;							/* under lying file type */
    zFile_Dimension var_dim_type;						/* dimension type */

    xmlDocPtr var_xml_doc_ptr;							/* xml document pointer */
    
    double var_width;								/* width of the sheet */
    double var_size;								/* height of the sheet */

    double var_scale;
    void* var_child;								/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif 

    /* constructor and destructor */
    zSurface* zsurface_new(const char* file_name, zFile_Type file_type);
    void zsurface_delete(zSurface* obj);

    /* create a cairo surface */
    int zsurface_create_cairo_surface(zSurface* obj, double width, double height);

#define zsurface_set_dim_type(obj, dim_type) \
    if(!(obj)) \
	(obj)->var_dim_type = dim_type

    
#ifdef __cplusplus
}
#endif
#endif /* _ZSURFACE_H_ */
