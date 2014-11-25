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
#define ZGENERIC_INIT_VTABLE(obj)				\
	(obj)->vtable.zgeneric_draw = NULL;			\
	(obj)->vtable.zgeneric_delete = NULL;		\
	(obj)->vtable.zgeneric_new = NULL


/* forward declaration of generic object */
typedef struct _zgeneric zgeneric;

/* vtable for element classes */
struct _zgeneric_vtable
{
	int (*zgeneric_draw)(void*);
	int (*zgeneric_delete)(void*);
	int (*zgeneric_new)(void*);
};

struct _zgeneric
{
	unsigned int _int_flg;				/* internal flag */
	unsigned int def_dev_ctxt_flg;		/* defulat device context set */
	zdevice* gdev;						/* primary device */
	void* child;						/* child sheet object */

	cairo_t* gcairo_dev;				/* cairo device */

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

		/* check if device context was set in
		   device object */
		if(obj->gdev->device)
			{
				obj->gcairo_dev = obj->gdev->device;
				obj->def_dev_ctxt_flg = 1;
			}

		return ZELIA_OK;
	}

	/* set defaults */
	inline __attribute__ ((always_inline)) static int zgeneric_set_defauts(zgeneric* obj)
	{
		/* check for NULL pointer */
		Z_CHECK_OBJ(obj);
		obj->gdev = zdevice_get_context(obj->gdev);
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
#define zgeneric_get_linecolour_ix(obj)			\
	zdevice_get_linecolourix((obj)->gdev)
#define zgeneric_device_set_linettype(obj, var)	\
	zdevice_set_linettype((obj)->gdev, var)
#define zgeneric_device_get_linettype(obj)		\
	zdevice_get_linettype((obj))

	/* helper macros for setting function pointers */
#define zgeneric_set_new_callback(obj, callback)	\
	(obj)->parent.vtable.zgeneric_new = (callback)
#define zgeneric_set_delete_callback(obj, callback)		\
	(obj)->parent.vtable.zgeneric_delete = (callback)
#define zgeneric_set_draw(obj, callback)			\
	(obj)->parent.vtable.zgeneric_draw = (callback)

#ifdef __cplusplus
}
#endif

#endif	/* __ZGENERIC __*/
