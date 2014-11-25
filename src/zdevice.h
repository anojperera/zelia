/* Declaration of device object. Device object shall be
   the primary object to be associated with all other
   objects
   Sat Oct 16 12:28:32 BST 2010
*/

#ifndef __ZDEVICE__
#define __ZDEVICE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <cairo/cairo.h>
#include <cairo/cairo-svg.h>

#include "zVar.h"

#define ZDEVICE_TEMP_FILE_SZ 256

/* forward declaration */
typedef struct _zdevice zdevice;

struct _zdevice
{
	unsigned int _int_flg;			/* internal flag */
	unsigned int ref_cnt;			/* reference counter */

	int _fd;					/* file descriptor read / write */
	char _fname[ZDEVICE_TEMP_FILE_SZ 256];	/* file name buffer */
	size_t _buff_sz;				/* buffer size */
	char* _buff;				/* temporary buffer contets */

	cairo_surface_t* surface;			/* cairo surface */
	cairo_t* device;				/* cairo default device */

	zLineColour line_color_ix;			/* line color index */
	zLineTypes line_type_ix;				/* line type index */
	zSheets page_sz;				/* page size */

	doubl red_rgb;				/* rgb red */
	double green_rgb;				/* rgb green */
	double blue_rgb;				/* rgb blue */

	double page_width;				/* page width */
	double page_height;				/* page height */
};

#ifdef __cplusplus
extern "C" {
#endif
	/* constructor and destructor */
	zdevice* zdevice_new(zdevice* obj);
	zdevice* zdevice_new2(zSheets sh,
						  int dev_flg,
						  zdevice* obj);

	void zdevice_delete(zdevice* obj);

	/* Property Methods */
	/********************************************************************/
	/* Set and get page size */
	int zdevice_set_pagesize(zdevice* obj, zSheets var);

	/* get page size */
	inline __attribute__ ((always_inline)) static zSheets zdevice_get_pagesize(zdevice* obj)
	{
		if(obj == NULL)
			return zSheetA3_Landscape;
		else
			return obj->z_page_sz;
	}


	/* Get default device context */
	cairo_t* zdevice_get_context(zdevice* obj);

	/* Set and get line colour */
	int zdevice_set_linecolourix(zdevice* obj, zLineColour var);

	/* get line color index */
	inline __attribute__ ((always_inline)) static zlinecolour zdevice_get_linecolourix(zdevice* obj)
	{
		/* check for NULL pointer */
		if(obj == NULL)
			return zLBlack;
		else
			return obj->z_line_color_ix;
	}

	/* get temporary buffer */
	const char* zdevice_get_temp_buff(zdevice* obj);

	/* set and get line types */
	int zdevice_set_linettype(zdevice* obj, zLineTypes var);
	inline __attribute__ ((always_inline)) static zLineTypes zdevice_get_linettype(zdevice* obj)
	{
		if(obj == NULL)
			return zLTContinuous;
		else
			return obj->line_type_ix;
	}
#ifdef __cplusplus
}
#endif

#endif	/* __ZDEVICE__ */
