/* Implementation of glands collection.
 * Fri Feb  3 17:58:12 GMT 2012 */

#include <blist.h>
#include "zgland.h"
#include "zglands.h"


/* Virtual functions */
static int _zglands_draw(void* obj);
static int _zglands_delete(void* obj);

/* Constructor */
zgenerics* zglands_new(zglands* obj)
{
    /* Create base constructor */
    ZCONSTRUCTOR(obj, zglands);

    /* Create parent object */
    if(!(obj->super_cls = zgenerics_new(&obj->parent, 1, 0)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* set properties */
    obj->child = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    /* set function pointers of parent object */
    zgeneric_set_delete_callback(obj, _zglands_delete);
    zgeneric_set_draw(obj, _zglands_draw);

    /* set child pointer */
    zgeneric_set_child_pointer(obj);

    /* return parent object */
    return obj->super_cls;
}

/* Destructor */
void zglands_delete(zglands* obj)
{
    ZCHECK_OBJ_VOID(obj);

    if(obj->vtable.zgeneric_delete)
	obj->vtable.zgeneric_delete((void*) obj->super_cls);
    else
	{
	    /* Call delete method of parent object */
	    zgenerics_delete(&obj->parent);
	}

    obj->child = NULL;
    obj->super_cls = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Add gland to collection */
int zglands_add(zglands* obj,
		zdevice* dev,				/* device object */
		double x,				/* x coord */
		double y,				/* y coord */
		zGlandSize sz,				/* gland size */
		unsigned int hex_flg)			/* hex flag */
{

    zgeneric* _zg = NULL;

    /* check for objects */
    ZCHECK_OBJ_INT(obj);
    _zg = zgland_new(NULL, dev, x, y, sz);

    /* set collection object pointer */
    zgeneric_set_collection_pointer(_zg, obj);

    if(dev == NULL)
	{
	    zgeneric_set_device(_zg, zgenerics_get_device(&obj->parent));
	    zgeneric_set_default_dev_context(_zg);
	}

    zgland_set_hex_profile_flag(Z_GLAND(_zg), hex_flg);

    /* add to collection */
    blist_add_from_end(&obj->parent.generics_d, _zg);

    return 0;
}


/*=================================== Private Methods ===================================*/

/* Virtual delete function */
static int _zglands_delete(void* obj)
{
    zgeneric* _zg;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    zgland_delete(Z_GLAND(_zg));

    return ZELIA_OK;
}


/* Virtual draw function */
static int _zglands_draw(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    return zgland_draw(Z_GLAND(_zg));
}
