/* implementation of generic object
   Sat Oct 16 17:51:25 BST 2010 */

#include "zgeneric.h"

/* constructor */
zgeneric* zgeneric_new(zgeneric* obj)
{
    /* check for object and and create it */
    ZCONSTRUCTOR(obj, zgeneric);

    obj->child = NULL;			/* set child pointer to NULL */
    obj->super_cls = obj;		/* pointer to itself */
    obj->cols = NULL;
    obj->gdev = NULL;
    obj->gcairo_dev = NULL;

    obj->def_dev_ctxt_flg = 0;
    obj->ref_flg = 0;
    
    /* initialise the vtable */
    ZGENERIC_INIT_VTABLE(obj);
    ZELIA_LOG_MESSAGE("zgeneric object created");
    return obj;

}

/* delete device */
void zgeneric_delete(zgeneric* obj)
{
    /* check for NULL pointer */
    ZCHECK_OBJ_VOID(obj);

    /*
     * If delete pointer was set call it first
     */
    if(obj->vtable.zgeneric_delete)
	{
	    obj->vtable.zgeneric_delete((void*) obj);
	    return;
	}

    /* delete the device object if it was internally created */
    zdevice_delete(obj->gdev);

    obj->gdev = NULL;
    obj->cols = NULL;
    obj->child = NULL;
    obj->super_cls = NULL;
    obj->gcairo_dev = NULL;
    obj->ref_flg = 0;
    
    /* remove vtables */
    ZGENERIC_INIT_VTABLE(obj);

    /* if the object was created free it */
    if(ZDESTRUCTOR_CHECK)
	free(obj);

    ZELIA_LOG_MESSAGE("zgeneric object deleted");
    return;
}

/*
 * Draw method of the super class.
 * This is equivalent to C++ virtual function
 */
int zgeneric_draw(zgeneric* obj)
{
    /* Check for object */
    ZCHECK_OBJ_INT(obj);
    ZCHECK_OBJ_INT(obj->vtable.zgeneric_draw);
    ZELIA_LOG_MESSAGE("zgeneric checking for child pointer and calling it");
    if(obj->vtable.zgeneric_draw)
	return obj->vtable.zgeneric_draw((void*) obj);
    else
	return ZELIA_OK;
}

/* create generic device context */
int zgeneric_create_dev_context(zgeneric* obj, zSheets sh)
{
    /* check for NULL pointer */
    ZCHECK_OBJ_INT(obj);

    ZELIA_LOG_MESSAGE("zgeneric creating default context");
    /*
     * If the device object is already in use,
     * we delete it first.
     */
    zdevice_delete(obj->gdev);

    obj->gdev = zdevice_new2(sh,					/* sheet size */
			     1,						/* create a device context */
			     NULL);					/* no external device pointer */

    /* set flag to indicate context was created internally */
    obj->def_dev_ctxt_flg = 1;
    ZELIA_LOG_MESSAGE("zgeneric default context created");
    return ZELIA_OK;
}
