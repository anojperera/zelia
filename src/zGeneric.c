/*
 * Implementation of the generic class.
 * Sun Jul 27 01:47:11 BST 2014
 */
#include <stdlib.h>
#include <string.h>
#include "zGeneric.h"

/* constructor */
zGeneric* zgeneric_new(zGeneric* obj, cairo_t* cairo_dev)
{
    /* constructor helper macro */
    ZCONSTRUCTOR(obj, zGeneric);

    obj->var_cairo_init = 0;
    obj->var_ref_cnt = 0;

    obj->var_x = 0.0;
    obj->var_y = 0.0;
    obj->var_z = 0.0;

    obj->var_width = 0.0;
    obj->var_height = 0.0;
    obj->var_depth = 0.0;

    obj->var_cairo_device_obj = NULL;
    obj->var_usurface = NULL;
    obj->var_child = NULL;
    
    /* initialise vtable */
    ZGENERIC_INIT_VTABLE(&obj->var_vtable);

    return obj;
}

/* destructor */
void zgeneric_delete(zGeneric* obj)
{
    /* check objects */
    ZCHECK_OBJ_VOID(obj);

    /* if the destructor of child pointer set call it */
    if(obj->var_vtable._vdelete)
	obj->var_vtable._vdelete(obj->var_child);

    /*
     * if reference counter is greater than 0,
     * we then return by decrementing the counter
     */
    if((obj->var_ref_cnt--) > 0)
	return;
    
    if(obj->var_init_flg == Z_OBJECT_INTERNAL)
	free(obj);
    return;
}

/* draw method */
int zgeneric_draw(zGeneric* obj)
{
    /* check object */
    ZCHECK_OBJ_INT(obj);

    /* if the drawing function pointer was set we call it */
    if(obj->var_vtable._vdraw)
	return obj->var_vtable._vdraw(obj->var_child);

    return 0;
}


