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
#include "zdevice.h"

/* initialise vtable */
#define ZGENERIC_INIT_VTABLE(obj)		\
    (obj)->z_vtable.zgeneric_draw = NULL;	\
    (obj)->z_vtable.zgeneric_delete = NULL;	\
    (obj)->z_vtable.zgeneric_new = NULL


/* forward declaration of generic object */
typedef struct _zgeneric zgeneric;

/* vtable for element classes */
struct _zgeneric_vtable
{
    int (*zgeneric_draw)(zgeneric*);
    int (*zgeneric_delete)(zgeneric*);
    int (*zgeneric_new)(zgeneric*);
};

struct _zgeneric
{
    zdevice* z_gdev;				/* primary device */
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
    
    struct _zgeneric_vtable z_vtable;		/* vtable for element object classes */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif
    /* create and delete object */
    zgeneric* zgeneric_new(zgeneric* obj);
    void zgeneric_delete(zgeneric* obj);

    /* Create generic device context.
     * This function is used for creating a device context
     * unique to this object.
     * Default shall be to use the device context of zDevice */
    int zgeneric_create_dev_context(zgeneric* obj);

    /* draw component */
    int zgeneric_draw(zgeneric* obj);

    /* Property Methods */
    /***************************************************************/
    /* Set and get device */
    inline __attribute__ ((always_inline)) static int zgeneric_set_device(zgeneric* obj, zdevice* var)
    {
	/* check for arguments */
	Z_CHECK_OBJ(obj);
	Z_CHECK_OBJ(var);

	obj->z_gdev = var;

	/* Set to default context */
	return 0;
    }
    
    inline __attribute__ ((always_inline)) static zdevice* zgeneric_get_device(zgeneric* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);
	return obj->z_gdev;
    }

    /* Set and get line colour ix */
    inline int zgeneric_set_linecolour(zgeneric* obj, zlinecolour var);
    inline __attribute__ ((always_inline)) static zlinecolour zgeneric_get_linecolour(zgeneric* obj)
    {
	/* check for NULL pointer */
	if(obj == NULL)
	    return zLBlack;
	else
	    return obj->z_gline_color_ix;
    }

    /* Set and line weight */
    int zgeneric_set_lineweight(zgeneric* obj, zlineweights var);
    inline __attribute__ ((always_inline)) static zlineweights zgeneric_get_lineweight(zgeneric* obj)
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
    inline __attribute__ ((always_inline)) static int zgeneric_set_default_dev_context(zgeneric* obj)
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
    int zgeneric_set_defauts(zgeneric* obj);
	
    /* get device context */
    inline __attribute__ ((always_inline)) static cairo_t* zgeneric_get_dev_context(zgeneric* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);

	return obj->z_gcairo_dev;
    }
	
    /* set and get line type */
    inline int zgeneric_set_linttype(zgeneric* obj, zlinetypes var);
    inline __attribute__ ((always_inline)) static zlinetypes zgeneric_get_linetype(zgeneric* obj)
    {
	/* check for NULL pointer */
	if(obj == NULL)
	    return zLTContinuous;
	else
	    return obj->z_gltype;	
    }

    /* helper macros for setting function pointers */
#define zgeneric_set_new_callback(obj, callback)	\
    (obj)->z_parent.z_vtable.zgeneric_new = (callback)
#define zgeneric_set_delete_callback(obj, callback)		\
    (obj)->z_parent.z_vtable.zgeneric_delete = (callback)
#define zgeneric_set_draw(obj, callback)		\
    (obj)->z_parent.z_vtable.zgeneric_draw = (callback)
    
#ifdef __cplusplus
}
#endif

#endif	/* __ZGENERIC __*/
