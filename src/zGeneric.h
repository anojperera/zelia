/*
 * Generic class which is inherited by all child classes.
 * Defines all interfaces.
 * Sun Jul 27 00:02:27 BST 2014
 */
#ifndef _ZGENERIC_H_
#define _ZGENERIC_H_


#include <cairo/cairo.h>
#include "zVar.h"
#include "zSurface.h"


/* Macro for initialising the VTABLE */
#define ZGENERIC_INIT_VTABLE(obj)		\
    (obj)->_vnew = NULL;			\
	 (obj)->_vdelete = NULL;		\
	 (obj)->_vdraw = NULL;			\
	 (obj)->_vsave = NULL

/* function pointer array */
struct _zgeneric_vtable
{
    int (*_vnew)(void*);
    int (*_vdelete)(void*);    
    int (*_vdraw)(void*);
    int (*_vsave)(void*);
};


/* type def the class struct */
typedef struct _zgeneric zgeneric;

struct _zgeneric
{
    unsigned int var_init_flg;						/* flag to indicate initialised */

    /*
     * This flag indicate to the class that cairo device object has been
     * locally initialised and has to be removed locally.
     */
    unsigned int var_cairo_init;

    unsigned int var_ref_cnt;						/* reference counter */

    double var_x;
    double var_y;
    double var_z;

    double var_width;
    double var_height;
    double var_depth;
    

    /*
     * Cairo device object either can be created for the
     * underlying surface or can be passed externally
     */
    cairo_t* var_cairo_device_obj;

    const zSurface* var_usurface;						/* pointer to the underlying surface */
    struct _zgeneric_vtable var_vtable;					/* function pointer array */
    void* var_child;							/* child pointer */
};


#ifdef __cplusplus
extern "C" {
#endif

    /* constructor and destructor */
    zGeneric* zgeneric_new(zGeneric* obj, cairo_t* cairo_dev);
    void zgeneric_delete(zGeneric* obj);

    /* below are generic methods */
    int zgeneric_draw(zGeneric* obj);

    /* set coordinates */
    inline __attribute__ ((always_inline)) static int zgeneric_set_cords(zGeneric* obj,
									double x,
									double y,
									double z)
    {
	if(obj != NULL)
	    {
		obj->var_x = x;
		obj->var_y = y;
		obj->var_z = z;
		return 0;
	    }
	return -1;
    }
    
#define zgeneric_get_x(obj)			\
    ((obj)? (obj)->var_x : 0.0)
#define zgeneric_get_y(obj)			\
    ((obj)? (obj)->var_y : 0.0)
#define zgeneric_get_z(obj)			\
    ((obj)? (obj)->var_z : 0.0)

    /* set dimensions */
    inline __attribute__ ((always_inline)) static int zgeneric_set_dims(zGeneric* obj,
									double width,
									double height,
									double depth)
    {
	if(!obj)
	    {
		obj->var_width = NULL;
		obj->var_height = NULL;
		obj->var_depth = NULL;
		return 0;
	    }
	return -1;
    }
#define zgeneric_get_width(obj)			\
    ((obj)? (obj)->var_width : 0.0)
#define zgeneric_get_height(obj)		\
    ((obj)? (obj)->var_height : 0.0)
#define zgeneric_get_depth(obj)			\
    ((obj)? (obj)->var_depth : 0.0)


    /* helper macros for setting function pointers of parent object */
    /* set new helper method */
#define zgeneric_set_vnew(obj, fn)		\
    (obj)->var_parent.var_vtable._vnew = (fn)
#define zgeneric_set_vdelete(obj, fn)			\
    (obj)->var_parent.var_vtable._vdelete = (fn)
#define zgeneric_set_vdraw(obj, fn)		\
    (obj)->var_parent.var_vtable._vdraw = (fn)
#define zgeneric_set_vsave(obj, fn)		\
    (obj)->var_parent.var_vtable._vsave = (fn)
#ifdef __cplusplus
}
#endif
#endif /* _ZGENERIC_H_ */


