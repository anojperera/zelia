/* Implementation of glands collection.
 * Fri Feb  3 17:58:12 GMT 2012 */

#include "zGlands.h"

/* Virtual functions */
static int _zglands_draw(zGenerics* obj);

/* Constructor */
zGenerics* zGlands_New(zGlands* obj)
{
    /* Create base constructor */
    if(obj == NULL)
	{
	    obj = (zGlands*) malloc(sizeof(zGlands));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create parent object */
    if(zGenerics_New(&obj->z_parent, 1, 0))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* set properties */
    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* set function pointers of parent object */
    obj->z_parent.z_draw_func = _zglands_draw;
    obj->z_parent.z_destructor_func = _zglands_delete;
    obj->z_parent.z_child = (void*) obj;
    obj->z_obj_sz = sizeof(zGlands);

    /* return parent object */
    return &obj->z_parent;
}

/* Destructor */
void zGlands_Delete(zGlands* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call delete method of parent object */
    zGenerics_Delete(&obj->z_parent);

    obj->z_child = NULL;
    if(obj->z_int_flg)
	free(obj);
}

