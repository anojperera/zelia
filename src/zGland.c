/* Implementation of zGland */
/* Fri Feb  3 14:35:35 GMTST 2012 */

#include "zGland.h"

/* Virtual functions */
static int _zgland_draw(zGeneric* obj);

/* Constructor */
zGeneric* zGland_New(zGland* obj,		/* optional object */
		     zDevice* dev,		/* optional device object */
		     zGlandSize sz)
{
    /* Check object */
    if(obj == NULL)
	{
	    obj = (zGland*) malloc(sizeof(zGland));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create base object */
    if(!zBase_New(&obj->z_parent))
	{
	    free(obj);
	    obj = NULL;
	    return NULL;
	}

    /* Set device */
    if(dev)
	{
	    zGeneric_Set_Device(&obj->z_parent.z_sgeneric, dev);
	    zGeneric_Set_Default_Dev_Context(&obj->z_parent.z_sgeneric);
	}

    /* set properties */
    obj->z_gland_size = sz;
    switch(sz)
	{
	case zM16:
	    obj->z_dia = Z_M16_GLAND;
	    break;

	case zM20:
	    obj->z_dia = Z_M20_GLAND;
	    break;
	case zM25:
	    obj->z_dia = Z_M25_GLAND;
	    break;
	default:
	    obj->z_dia = Z_M20_GLAND;
	}

    obj->z_draw_func = NULL;
    
    /* Set function pointers of parent object */
    obj->z_parent.z_draw_func = _zgland_draw;

    /* Set child pointer of parent object and return
     * top level object */
    obj->z_parent.z_child = (void*) obj;

    return &obj->z_parent.z_sgeneric;
}


/* Destructor */
void zGland_Delete(zGland* obj)
{

}
