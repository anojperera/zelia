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

/* forward declaration of generic object */
typedef struct _zGeneric zGeneric;

/* Declaration of callback function */
typedef int (*zgeneric_fptr) (zGeneric* obj);
struct _zGeneric
{
    zDevice* z_gdev;				/* primary device */
    void* z_child;				/* child sheet object */
		
    cairo_t* z_gcairo_dev;			/* cairo device */
	
    double z_gred_rgb;				/* rgb red */
    double z_ggreen_rgb;			/* rgb green */
    double z_gblue_rgb;				/* rgb blue */

    zLineColour z_gline_color_ix;		/* line color index */
    zLineWeights z_gline_weight;		/* line weight */
    zLineTypes z_gltype;			/* line type */
    unsigned int z_int_flg;			/* internal flag */
    unsigned int z_def_dev_ctxt_flg;		/* defulat device context set */
    
    zgeneric_fptr z_draw_func;			/* draw function pointer */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif
    /* create and delete object */
    zGeneric* zGeneric_New(zGeneric* obj);
    void zGeneric_Delete(zGeneric* obj);

    /* Create generic device context.
     * This function is used for creating a device context
     * unique to this object.
     * Default shall be to use the device context of zDevice */
    int zGeneric_Create_Dev_Context(zGeneric* obj);

    /* Draw component */
    int zGeneric_Draw(zGeneric* obj);

    /* Property Methods */
    /***************************************************************/
    /* Set and get device */
    inline __attribute__ ((always_inline)) static int zGeneric_Set_Device(zGeneric* obj, zDevice* var)
    {
	/* check for arguments */
	Z_CHECK_OBJ(obj);
	Z_CHECK_OBJ(var);

	obj->z_gdev = var;

	/* Set to default context */
	return 0;
    }
    
    inline __attribute__ ((always_inline)) static zDevice* zGeneric_Get_Device(zGeneric* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);
	return obj->z_gdev;
    }

    /* Set and get line colour ix */
    inline int zGeneric_Set_LineColour(zGeneric* obj, zLineColour var);
    inline __attribute__ ((always_inline)) static zLineColour zGeneric_Get_LineColour(zGeneric* obj)
    {
	/* check for NULL pointer */
	if(obj == NULL)
	    return zLBlack;
	else
	    return obj->z_gline_color_ix;
    }

    /* Set and line weight */
    int zGeneric_Set_LineWeight(zGeneric* obj, zLineWeights var);
    inline __attribute__ ((always_inline)) static zLineWeights zGeneric_Get_LineWeight(zGeneric* obj)
    {
	/* check NULL pointer */
	if(obj == NULL)
	    return zLWeight1;
	else
	    return obj->z_gline_weight;	
    }

    /* Set default device context.
     * Function is used for setting the device context to
     * zDevice */
    inline __attribute__ ((always_inline)) static int zGeneric_Set_Default_Dev_Context(zGeneric* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ(obj);

	/* check for device context */
	Z_CHECK_OBJ(obj->z_gdev);

	/* check if device context was set in
	   device object */
	if(obj->z_gdev->z_device)
	    {
		obj->z_gcairo_dev = obj->z_gdev->z_device;
		obj->z_def_dev_ctxt_flg = 1;
	    }
    
	return 0;
    }

    /* set defaults */
    int zGeneric_Set_Defauts(zGeneric* obj);
	
    /* get device context */
    inline __attribute__ ((always_inline)) static cairo_t* zGeneric_Get_Dev_Context(zGeneric* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);

	return obj->z_gcairo_dev;
    }
	
    /* set and get line type */
    inline int zGeneric_Set_LintType(zGeneric* obj, zLineTypes var);
    inline __attribute__ ((always_inline)) static zLineTypes zGeneric_Get_LineType(zGeneric* obj)
    {
	/* check for NULL pointer */
	if(obj == NULL)
	    return zLTContinuous;
	else
	    return obj->z_gltype;	
    }

    
#ifdef __cplusplus
}
#endif

#endif	/* __ZGENERIC __*/
