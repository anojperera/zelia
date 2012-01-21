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
	unsigned int var_int_flg;		/* internal flag */
	cairo_surface_t* z_surface;		/* cairo surface */
	cairo_t* z_device;			/* cairo default device */

	zLineColour z_line_color_ix;		/* line color index */
	zSheets z_page_sz;			/* page size */
	zOutputFormat z_format;			/* out put format */
	char z_filename[256];			/* file name */
	char z_page_sz_str[16];			/* page size string */
	double z_red_rgb;			/* rgb red */
	double z_green_rgb;			/* rgb green */
	double z_blue_rgb;			/* rgb blue */

	double z_page_width;			/* page width */
	double z_page_height;			/* page height */


    };

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
    inline zOutputFormat zDevice_Get_SurfaceType(zDevice* obj);

    /* Set and get page size */
    inline int zDevice_Set_PageSize(zDevice* obj,
				    zSheets var);
    inline zSheets zDevice_Get_PageSize(zDevice* obj);

    /* Returns a pointer to page size string */
    inline char* zDevice_Get_PageSizeStr(zDevice* obj);

    /* Set and get file name */
    inline int zDevice_Set_FileName(zDevice* obj,
				    const char* var);
    inline char* zDevice_Get_FileName(zDevice* obj);
		
    /* Get default device context */
    inline cairo_t* zDevice_Get_Context(zDevice* obj); 

    /* Set and get line colour */
    inline int zDevice_Set_LineColourIx(zDevice* obj,
					zLineColour var);
    inline zLineColour zDevice_Get_LineColourIx(zDevice* obj); 

	
#ifdef __cplusplus
}
#endif

#endif	/* __ZDEVICE__ */
