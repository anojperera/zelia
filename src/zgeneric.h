/*
 * generic class to be inherited by all classes
 * Sat Oct 16 16:40:47 BST 2010
 */

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
#define ZGENERIC_INIT_VTABLE(obj)			\
    (obj)->vtable.zgeneric_draw = NULL;			\
    (obj)->vtable.zgeneric_delete = NULL;		\
    (obj)->vtable.zgeneric_new = NULL;			\
    (obj)->vtable.zgeneric_auth_default_device = NULL


/* forward declaration of generic object */
typedef struct _zgeneric zgeneric;

/* vtable for element classes */
struct _zgeneric_vtable
{
    int (*zgeneric_draw)(void*);
    int (*zgeneric_delete)(void*);
    int (*zgeneric_new)(void*);
    int (*zgeneric_auth_default_device)(void*);
};

struct _zgeneric
{
    unsigned int _init_flg;			/* internal flag */
    unsigned int def_dev_ctxt_flg;		/* defulat device context set */
    unsigned int ref_flg;			/* reference flag */
    zdevice* gdev;				/* primary device */

    zgeneric* super_cls;			/* pointer to itself */
    void* cols;					/* pointer this objects collection */
    void* child;				/* child sheet object */

    cairo_t* gcairo_dev;			/* cairo device */

    struct _zgeneric_vtable vtable;		/* vtable for element object classes */
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
    int zgeneric_create_dev_context(zgeneric* obj, zSheets sh);

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
	return ZELIA_OK;
    }

    inline __attribute__ ((always_inline)) static zdevice* zgeneric_get_device(zgeneric* obj)
    {
	/* check for NULL pointer */
	ZCHECK_OBJ_PTR(obj);
	return obj->gdev;
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

	/*
	 * First check if authorise callback was assigned.
	 * We call it to check it returns ZELIA_OK.
	 * If it doesn't that means the ref count shouldn't be incremented
	 */
	if(obj->vtable.zgeneric_auth_default_device &&
	   obj->vtable.zgeneric_auth_default_device((void*) obj) != ZELIA_OK)
	    return ZELIA_UNAUTHORISE;


	/* check if device context was set in
	   device object */
	if(obj->gdev->device)
	    {
		obj->gcairo_dev = zdevice_get_context(obj->gdev);
		obj->def_dev_ctxt_flg = 0;
	    }

	return ZELIA_OK;
    }

    /* set defaults */
    inline __attribute__ ((always_inline)) static int zgeneric_set_defauts(zgeneric* obj)
    {
	/* check for NULL pointer */
	ZCHECK_OBJ_INT(obj);
	return ZELIA_OK;
    }

    /* get device context */
    inline __attribute__ ((always_inline)) static cairo_t* zgeneric_get_dev_context(zgeneric* obj)
    {
	/* check for NULL pointer */
	ZCHECK_OBJ_PTR(obj);

	return obj->gcairo_dev;
    }


    /*
     * Helper macros for setting colour and line types.
     * These wrap device objects methods.
     */
#define zgeneric_set_linecolour_ix(obj, var)		\
    zdevice_set_linecolourix((obj)->gdev, (var))
#define zgeneric_get_linecolour_ix(obj)		\
    zdevice_get_linecolourix((obj)->gdev)
#define zgeneric_device_set_linettype(obj, var)	\
    zdevice_set_linettype((obj)->gdev, var)
#define zgeneric_device_get_linettype(obj)	\
    zdevice_get_linettype((obj))

    /* helper macros for setting function pointers */
#define zgeneric_set_new_callback(obj, callback)	\
    (obj)->super_cls->vtable.zgeneric_new = (callback)
#define zgeneric_set_delete_callback(obj, callback)		\
    (obj)->super_cls->vtable.zgeneric_delete = (callback)
#define zgeneric_set_draw(obj, callback)		\
    (obj)->super_cls->vtable.zgeneric_draw = (callback)
#define zgeneric_set_device_auth_default_callback(obj, callback)	\
    (obj)->super_cls->vtable.zgeneric_auth_default_device = (callback)

    /* helper macro for setting the child pointer */
#define zgeneric_set_child_pointer(obj)		\
    (obj)->parent.child = (void*) (obj)

#define zgeneric_set_collection_pointer(obj, collection)	\
    (obj)->cols = (collection)
#define zgeneric_get_collection_pointer(obj)	\
    ((obj)? (obj)->super_cls->cols : NULL)

    /* helper macro to indicate parent called the destructor */
#define zgeneric_called_by_parent(obj)					\
    ((obj)->super_cls->vtable.zgeneric_delete? ZELIA_OK : ZELIA_NULL)
#define zgeneric_block_parent_destructor(obj)		\
    (obj)->super_cls->vtable.zgeneric_delete = NULL

#define zgeneric_toggle_ref_flg(obj)		\
    if((obj)->ref_flg > 0)			\
	(obj)->ref_flg = 0;			\
    else					\
	(obj)->ref_flg = 1

#define zgeneric_get_ref_flg(obj)		\
    (obj)->ref_flg

#ifdef __cplusplus
}
#endif

#endif	/* __ZGENERIC __*/
