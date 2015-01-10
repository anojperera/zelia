/* Implementation of zBase class
 * Sat Jan 21 16:23:09 GMT 2012 */

#include "zbase.h"

/* Virtual function call back */
static int _zbase_delete_function(void* obj);


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

    /* set parent objects function pointers */
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

    zgeneric_block_parent_destructor(obj);
    zgeneric_delete(&obj->parent);

    obj->child = NULL;
    obj->super_cls = NULL;

    /* check if the object was constructed */
    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/*=================================== Private Methods ===================================*/

/* delete callback method from parent */
static int _zbase_delete_function(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;
    zbase_delete(Z_BASE(_zg));

    return ZELIA_OK;
}
