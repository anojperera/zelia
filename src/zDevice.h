/* Declaration of device object. Device object shall be
   the primary object to be associated with all other
   objects
   Sat Oct 16 12:28:32 BST 2010 */

#ifndef __ZDEVICE__
#define __ZDEVICE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <cairo/cairo.h>
#include <cairo/cairo-svg.h>
#include <cairo/cairo-pdf.h>

#include "zVar.h"

#define Z_DEVICE_FNAME_SZ 256
#define Z_DEVICE_PAGE_STR_SZ 16
/* forward declaration */
typedef struct _zDevice zDevice;

struct _zDevice
{
    unsigned int var_int_flg;			/* internal flag */
    unsigned int var_ref_cnt;			/* reference counter */
    cairo_surface_t* z_surface;			/* cairo surface */
    cairo_t* z_device;				/* cairo default device */

    zLineColour z_line_color_ix;		/* line color index */
    zSheets z_page_sz;				/* page size */
    zOutputFormat z_format;			/* out put format */
    char z_filename[Z_DEVICE_FNAME_SZ];		/* file name */
    char z_page_sz_str[Z_DEVICE_PAGE_STR_SZ];	/* page size string */
    double z_red_rgb;				/* rgb red */
    double z_green_rgb;				/* rgb green */
    double z_blue_rgb;				/* rgb blue */

    double z_page_width;			/* page width */
    double z_page_height;			/* page height */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif
    /* constructor and destructor */
    zDevice* zDevice_New(zDevice* obj);
    zDevice* zDevice_New2(zOutputFormat fm,
			  zSheets sh,
			  const char* fn,
			  int dev_flg,
			  zDevice* obj);
	
    void zDevice_Delete(zDevice* obj);

    /* Property Methods */
    /********************************************************************/

    /* Set and get type of surface */
    inline int zDevice_Set_SurfaceType(zDevice* obj,
				       zOutputFormat var);
    inline __attribute__ ((always_inline)) static zOutputFormat zDevice_Get_SurfaceType(zDevice* obj)
    {
	if(obj == NULL)
	    return zFormatPDF;
	else
	    return obj->z_format;
    }

    /* Set and get page size */
    inline __attribute__ ((always_inline)) static int zDevice_Set_PageSize(zDevice* obj, zSheets var)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ(obj);

	obj->z_page_sz = var;
	zdevice_page_dims(obj);
	return 0;
    }

    /* get page size */
    inline __attribute__ ((always_inline)) static zSheets zDevice_Get_PageSize(zDevice* obj)
    {
	if(obj == NULL)
	    return zSheetA3_Landscape;
	else
	    return obj->z_page_sz;
    }

    /* Returns a pointer to page size string */
    inline __attribute__ ((always_inline)) static char* zDevice_Get_PageSizeStr(zDevice* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);
	return obj->z_page_sz_str;
    }

    /* Set and get file name */
    inline __attribute__ ((always_inline)) static int zDevice_Set_FileName(zDevice* obj, const char* var)
    {
	/* check for object NULL pointer */
	Z_CHECK_OBJ(obj);
	Z_CHECK_OBJ(var);

	strncpy(obj->z_filename, var, Z_DEVICE_FNAME_SZ);

	return 0;
    }

    /* Get file name */
    inline __attribute__ ((always_inline)) static char* zDevice_Get_FileName(zDevice* obj)
    {
	Z_CHECK_OBJ_PTR(obj);
	return obj->z_filename;
    }
		
    /* Get default device context */
    inline __attribute__ ((always_inline)) static cairo_t* zDevice_Get_Context(zDevice* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);

	zdevice_create_context(obj);
	return obj->z_device;
    }

    /* Set and get line colour */
    inline __attribute__ ((always_inline)) static int zDevice_Set_LineColourIx(zDevice* obj, zLineColour var)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ(obj);

	obj->z_line_color_ix = var;
	zdevice_rgb(obj);
	return 0;
    }

    /* get line color index */
    inline __attribute__ ((always_inline)) static zLineColour zDevice_Get_LineColourIx(zDevice* obj)
    {
	/* check for NULL pointer */
	if(obj == NULL)
	    return zLBlack;
	else
	    return obj->z_line_color_ix;
    }

	
#ifdef __cplusplus
}
#endif

#endif	/* __ZDEVICE__ */
