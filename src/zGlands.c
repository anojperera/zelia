/* Implementation of glands collection.
 * Fri Feb  3 17:58:12 GMT 2012 */

#include "zGland.h"
#include "zGlands.h"
#include "../../g_list/src/alist.h"

/* Virtual functions */
static int _zglands_draw(void* obj, void* usr_data);
static int _zglands_delete(void* obj, void* usr_data);

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

/* Add gland to collection */
int zGlands_Add(zGlands* obj,				
		zDevice* dev,				/* device object */
		double x,				/* x coord */
		double y,				/* y coord */
		zGlandSize sz,				/* gland size */
		unsigned int hex_flg)			/* hex flag */
{
    zGland gland;
    zGeneric* zg;
    /* check for objects */
    Z_CHECK_OBJ(obj);
    zg = zGland_New(&gland, dev, x, y, sz);

    /* Set internal flag to true as the struct shall be copied to
     * internal memory of alist and it needs freed */
    gland.z_int_flg = 1;
    if(!zg)
	return 1;
    
    if(dev == NULL)
	{
	    zGeneric_Set_Device(zg, zGenerics_Get_Device(&obj->z_parent));
	    zGeneric_Set_Default_Dev_Context(zg);
	}
    
    zGland_Set_Hex_Profile_Flag(&gland, hex_flg);

    /* add to collection */
    aList_Add(&obj->z_parent.z_generics_d,
	      (void*) &gland,
	      sizeof(zGland));
    
    return 0;
}


/*=========================================================================*/
/* Private functions */

/* Virtual delete function */
static int _zglands_delete(void* obj, void* usr_data)
{
    if(obj)
	zGland_Delete(Z_GLAND(zgl));

    return 0;
}


/* Virtual draw function */
static int _zglands_draw(void* obj, void* usr_data)
{
    Z_CHECK_OBJ(obj);
    return zGland_Draw(Z_GLAND(zgl));
}
