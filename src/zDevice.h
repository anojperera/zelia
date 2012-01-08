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

#ifdef __cplusplus
extern "C" {
#endif


    /* forward declaration */
    typedef struct _zDevice zDevice;

    struct _zDevice
    {
	cairo_surface_t* z_surface;		/* cairo surface */
	cairo_t* z_device;			/* cairo default device */

	zLineColour z_line_color_ix;		/* line color index */
	zSheets z_page_sz;			/* page size */
	zOutputFormat z_format;			/* out put format */
	char* z_filename;			/* file name */
	char* z_page_sz_str;			/* page size string */
	double z_red_rgb;			/* rgb red */
	double z_green_rgb;			/* rgb green */
	double z_blue_rgb;			/* rgb blue */

	double z_page_width;			/* page width */
	double z_page_height;			/* page height */


    };

    /* constructor and destructor */
    zDevice* zDevice_Create();
    zDevice* zDevice_Create2(zOutputFormat fm,
			     zSheets sh,const char* fn,
			     int dev_flg);
	
    void zDevice_Delete(zDevice** obj);

    /* set and get type of surface */
    void zDevice_Set_SurfaceType(zDevice* obj,
				 zOutputFormat var);
    zOutputFormat zDevice_Get_SurfaceType(zDevice* obj);

    /* set and get page size */
    void zDevice_Set_PageSize(zDevice* obj,
			      zSheets var);
    zSheets zDevice_Get_PageSize(zDevice* obj);

    /* returns a pointer to page size string */
    char* zDevice_Get_PageSizeStr(zDevice* obj);

    /* set and get file name */
    void zDevice_Set_FileName(zDevice* obj,
			      const char* var);
    char* zDevice_Get_FileName(zDevice* obj);
		
    /* create default device context */
    void zDevice_Create_Context(zDevice* obj);
    cairo_t** zDevice_Get_Context(zDevice* obj); 

    /* set and get line colour */
    void zDevice_Set_LineColourIx(zDevice* obj,
				  zLineColour var);
    zLineColour zDevice_Get_LineColourIx(zDevice* obj); 

    /* private functions */
    inline void zdevice_rgb(zDevice** obj);
    inline void zdevice_page_dims(zDevice** obj);
	
#ifdef __cplusplus
}
#endif

#endif	/* __ZDEVICE__ */
