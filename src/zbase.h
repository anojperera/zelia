/* Base object for deriving all component object,
 * Describes, primitive coordinates */
/* Sat Jan 21 16:05:52 GMT 2012 */

#ifndef _ZBASE_H_
#define _ZBASE_H_

#include <stdlib.h>
#include "zgeneric.h"

typedef struct _zbase zbase;

struct _zbase
{
    zgeneric parent;				/* inherited parent object */
    unsigned int _int_flg;			/* internal flag */


    /* base coordinates */
    double x;
    double y;
    double ang;					/* orientation angle */
    double width;				/* with */
    double height;				/* height */
    double thk;					/* thickness of material */
    unsigned int prj_flg;			/* projected flag */

    struct _zgeneric_vtable vtable;		/* vtable for update draw, delete and new */
    zgeneric* super_cls;			/* super class (in this case it will be the parent */
    void* child;				/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgeneric* zbase_new(zbase* obj);
    void zbase_delete(zbase* obj);

    /* Property functions */
    /*************************************************************/

    /* Set base coordinates */
    inline __attribute__ ((always_inline)) static int zbase_set_base_coords(zbase* obj, double x, double y)
    {
	/* check for object pointers */
	ZCHECK_OBJ_INT(obj);
	obj->x = x;
	obj->y = y;
	return ZELIA_OK;
    }

    /* Set orientation angle */
    inline __attribute__ ((always_inline)) static int zbase_set_orientation(zbase* obj, double ang)
    {
	ZCHECK_OBJ_INT(obj);
	obj->ang = ang;
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static double zbase_get_orientation(zbase* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->ang;
    }

    /* Set and Get width height */
    inline __attribute__ ((always_inline)) static int zbase_set_width_and_height(zbase* obj, double width, double height)
    {
	/* check for object */
	ZCHECK_OBJ_INT(obj);
	obj->width = width;
	obj->height = height;
	return ZELIA_OK;
    }

    inline __attribute__ ((always_inline)) static int zbase_set_width(zbase* obj, double width)
    {
	/* check object */
	ZCHECK_OBJ_INT(obj);
	obj->width = width;
	return ZELIA_OK;
    }

    inline __attribute__ ((always_inline)) static double zbase_get_width(zbase* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->width;
    }

    inline __attribute__ ((always_inline)) static int zbase_set_height(zbase* obj, double height)
    {
	ZCHECK_OBJ_INT(obj);
	obj->height = height;
	return 0;
    }
    inline __attribute__ ((always_inline)) static double zbase_get_height(zbase* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->height;
    }

    inline __attribute__ ((always_inline)) static int zbase_set_thickness(zbase* obj, double thk)
    {
	ZCHECK_OBJ_INT(obj);
	obj->thk = thk;
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static double zbase_get_thickness(zbase* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->thk;
    }

    inline __attribute__ ((always_inline)) static int zbase_set_projected_flag(zbase* obj, unsigned int flg)
    {
	ZCHECK_OBJ_INT(obj);
	obj->prj_flg = flg;
	return 0;
    }
    inline __attribute__ ((always_inline)) static unsigned int zbase_get_projected_flag(zbase* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->prj_flg;
    }

#ifdef __cplusplus
}
#endif

#endif /* _ZBASE_H_ */
