/* generic class to be inherited by all classes
   Sat Oct 16 16:40:47 BST 2010 */

#ifndef __ZGENERIC__
#define __ZGENERIC__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>

#include "zVar.h"
#include "zDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* forward declaration of generic object */
    typedef struct _zGeneric zGeneric;

    struct _zGeneric
    {
	zDevice* z_gdev;		/* primary device */
	void* z_child;			/* child sheet object */
		
	cairo_t* z_gcairo_dev;		/* cairo device */

	double z_gred_rgb;		/* rgb red */
	double z_ggreen_rgb;		/* rgb green */
	double z_gblue_rgb;		/* rgb blue */

	zLineColour z_gline_color_ix;	/* line color index */
	zLineWeights z_gline_weight;	/* line weight */
	zLineTypes z_gltype;		/* line type */
	unsigned int z_int_flg;		/* internal flag */
    };

    /* create and delete object */
    zGeneric* zGeneric_New(zGeneric* obj);
    void zGeneric_Delete(zGeneric* obj);

    /* Create generic device context */
    int zGeneric_Create_Dev_Context(zGeneric* obj);

    /* Property Methods */
    /***************************************************************/

    /* Set and get device */
    inline int zGeneric_Set_Device(zGeneric* obj, zDevice* var);
    inline zDevice* zGeneric_Get_Device(zGeneric* obj);

    /* Set and get line colour ix */
    inline int zGeneric_Set_LineColour(zGeneric* obj,
				 zLineColour var);
    inline zLineColour zGeneric_Get_LineColour(zGeneric* obj);

    /* Set and line weight */
    inline int zGeneric_Set_LineWeight(zGeneric* obj,
				 zLineWeights var);
    inline zLineWeights zGeneric_Get_LineWeight(zGeneric* obj);

    /* Set default device context */
    inline int zGeneric_Set_Default_Dev_Context(zGeneric* obj);

    /* set defaults */
    inline int zGeneric_Set_Defauts(zGeneric* obj);
	
    /* get device context */
    inline cairo_t* zGeneric_Get_Dev_Context(zGeneric* obj);
	
    /* set and get line type */
    inline int zGeneric_Set_LintType(zGeneric* obj, zLineTypes var);
    inline zLineTypes zGeneric_Get_LineType(zGeneric* obj);

    
#ifdef __cplusplus
}
#endif

#endif	/* __ZGENERIC __*/
