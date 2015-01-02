/* Implementation of zBase class
 * Sat Jan 21 16:23:09 GMT 2012 */

#include "zbase.h"

/* Virtual function call back */
static int _zbase_delete_function(void* obj);
static int _zbase_draw_function(void* obj);
static int _zbase_default_device_auth(void* obj);

/* Constructor */
zgeneric* zbase_new(zbase* obj)
{
    /* call constructor helper */
    ZCONSTRUCTOR(obj, zbase);

    /* create parent object */
    obj->super_cls = zgeneric_new(&obj->parent);

    /* set properties */
    obj->x = 0.0;
    obj->y = 0.0;
    obj->ang = 0.0;
    obj->width = 0.0;
    obj->height = 0.0;
    obj->thk = 0.0;
    obj->prj_flg = 0;
    obj->child = NULL;

    /* initialise vtable */
    ZGENERIC_INIT_VTABLE(obj);

    /* set parent objects function pointers */
    zgeneric_set_draw(obj, _zbase_draw_function);
    zgeneric_set_device_auth_default_callback(obj, _zbase_default_device_auth);
    zgeneric_set_delete_callback(obj, _zbase_delete_function);
    
    /* set child pointer of parent object */
    zgeneric_set_child_pointer(obj);

    /* return base pointer */
    return obj->super_cls;
}

/* Destructor */
void zbase_delete(zbase* obj)
{
    ZCHECK_OBJ_VOID(obj);
    /* if function pointer was set call it */
    if(obj->vtable.zgeneric_delete)
	{
	    obj->vtable.zgeneric_delete((void*) obj->super_cls);
	    return;
	}

    zgeneric_block_parent_destructor(obj);
    zgeneric_delete(&obj->parent);

    obj->child = NULL;
    obj->super_cls = NULL;

    /* remove vtables */
    ZGENERIC_INIT_VTABLE(obj);

    /* check if the object was constructed */
    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/*=================================== Private Methods ===================================*/

/* draw function */
static int _zbase_draw_function(void* obj)
{
    zgeneric* _zg = NULL;
    zbase* _self = NULL;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    _self = Z_BASE(_zg);

    if(_self->vtable.zgeneric_draw)
	return _self->vtable.zgeneric_draw(obj);
    else
	return ZELIA_OK;
}

/* authorisation for setting the default context */
static int _zbase_default_device_auth(void* obj)
{
    zgeneric* _zg = NULL;
    zbase* _self = NULL;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    _self = Z_BASE(_zg);
    if(_self->vtable.zgeneric_auth_default_device)
	return _self->vtable.zgeneric_auth_default_device(obj);

    return ZELIA_OK;
}

/* delete callback method from parent */
static int _zbase_delete_function(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;
    zbase_delete(Z_BASE(_zg));

    return ZELIA_OK;
}
