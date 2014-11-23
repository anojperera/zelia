/* generic class to be inherited by all classes
   Sat Oct 16 16:40:47 BST 2010 */

#ifndef __ZGENERIC__
#define __ZGENERIC__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cairo/cairo.h>
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
    zdevice* gdev;				/* primary device */
    void* child;				/* child sheet object */

    cairo_t* gcairo_dev;			/* cairo device */

    double gred_rgb;				/* rgb red */
    double ggreen_rgb;			/* rgb green */
    double gblue_rgb;				/* rgb blue */

    zLineColour gline_color_ix;		/* line color index */
    zLineWeights gline_weight;		/* line weight */
    zLineTypes gltype;			/* line type */
    unsigned int int_flg;			/* internal flag */
    unsigned int def_dev_ctxt_flg;		/* defulat device context set */

    struct _zgeneric_vtable vtable;		/* vtable for element object classes */
    size_t obj_sz;				/* object size */
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
	ZCHECK_OBJ_INT(obj);
	ZCHECK_OBJ_INT(var);

	obj->gdev = var;

	/* Set to default context */
	return 0;
    }

    inline __attribute__ ((always_inline)) static zdevice* zgeneric_get_device(zgeneric* obj)
    {
	/* check for NULL pointer */
	ZCHECK_OBJ_PTR(obj);
	return obj->gdev;
    }

    /* Set and get line colour ix */
    inline int zgeneric_set_linecolour(zgeneric* obj, zLineColour var);
    inline __attribute__ ((always_inline)) static zLineColour zgeneric_get_linecolour(zgeneric* obj)
    {
	/* check for NULL pointer */
	if(obj == NULL)
	    return zLBlack;
	else
	    return obj->gline_color_ix;
    }

    /* Set and line weight */
    int zgeneric_set_lineweight(zgeneric* obj, zlineweights var);
    inline __attribute__ ((always_inline)) static zLineWeights zgeneric_get_lineweight(zgeneric* obj)
    {
	/* check NULL pointer */
	if(obj == NULL)
	    return zLWeight1;
	else
	    return obj->gline_weight;
    }

    /* Set default device context.
     * Function is used for setting the device context to
     * zDevice */
    inline __attribute__ ((always_inline)) static int zgeneric_set_default_dev_context(zgeneric* obj)
    {
	/* check for NULL pointer */
	ZCHECK_OBJ_INT(obj);

	/* check for device context */
	ZCHECK_OBJ_INT(obj->gdev);

	/* check if device context was set in
	   device object */
	if(obj->gdev->device)
	    {
		obj->gcairo_dev = obj->gdev->device;
		obj->def_dev_ctxt_flg = 1;
	    }

	return 0;
    }

    /* set defaults */
    int zgeneric_set_defauts(zgeneric* obj);

    /* get device context */
    inline __attribute__ ((always_inline)) static cairo_t* zgeneric_get_dev_context(zgeneric* obj)
    {
	/* check for NULL pointer */
	ZCHECK_OBJ_PTR(obj);

	return obj->gcairo_dev;
    }

    /* set and get line type */
    inline int zgeneric_set_linttype(zgeneric* obj, zLineTypes var);
    inline __attribute__ ((always_inline)) static zLineTypes zgeneric_get_linetype(zgeneric* obj)
    {
	/* check for NULL pointer */
	if(obj == NULL)
	    return zLTContinuous;
	else
	    return obj->gltype;
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
