/* Implementation of shafts collection */
/* Wed Apr 11 23:33:30 BST 2012 */

#include "zShafts.h"
#include "zShaft.h"

/* Virtual functions */
static int _zshafts_draw(void* obj, void* usr_data);
static int _zshafts_delete(void* obj, void* usr_data);

/* Constructor */
zGenerics* zShafts_New(zShafts* obj,		/* optional self pointer */
		       zDevice* dev,	/* device context */
		       double x,		/* x coordinate */
		       double y,		/* y coordinate */
		       double width,	/* shaft width */
		       double dia,		/* shaft diameter */
		       unsigned int num_shafts,	/* number of shafts */
		       double pitch)	/* blade pitch */
{
    int i;
    double shft_y;
    
    /* Constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zShafts);

    /* create parent object */
    if(zGenerics_New(&obj->z_parent,
		     0,
		     num_shafts))
	{
	    /* If the object was created internally,
	       delete it */
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set device context */
    zGenerics_Set_Device(&obj->z_parent, dev);

    obj->z_x = x;
    obj->z_y = y;
    obj->z_width = width;
    obj->z_dia = dia;
    obj->z_pitch = pitch;
    obj->z_obj_sz = sizeof(zShafts);

    /* Create shafts collection */
    for(i=0, shft_y=y; i<num_shafts; i++, shft_y+=pitch)
	{
	    obj->z_parent.z_generics_s[i] = zShaft_New(NULL);

	    /* set coordinates */
	    zBase_Set_Base_Coords(Z_BASE(obj->z_parent.z_generics_s[i]),
				  obj->z_x,
				  shft_y);
	    zBase_Set_Width_and_Height(Z_BASE(obj->z_parent.z_generics_s[i]),
				       width,
				       dia);

	    zGeneric_Set_Device(obj->z_parent.z_generics_s[i], dev);
	    zGeneric_Set_Default_Dev_Context(obj->z_parent.z_generics_s[i]);	    
	}

    /* set function pointers of parent object */
    obj->z_parent.z_draw_func = _zshafts_draw;
    obj->z_parent.z_destructor_func = _zshafts_delete;
    obj->z_parent.z_child = (void*) obj;
    obj->z_parent.z_usr_data = (void*) obj;

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    return &obj->z_parent;
}

/* Destructor */
void zShafts_Delete(zShafts* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call delete method of parent object */
    zGenerics_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    if(obj->z_int_flg)
	free(obj);
}

/* Set base coordinates */
inline int zShafts_Set_Coordinates(zShafts* obj, double x, double y)
{
    int i;
    double shft_y;
    
    Z_CHECK_OBJ(obj);

    for(i=0,shft_y=y; i<obj->z_parent.z_count; i++,shft_y+=obj->z_pitch)
	{
	    zBase_Set_Base_Coords(Z_BASE(obj->z_parent.z_generics_s[i]),
				  obj->z_x,
				  shft_y);
	}
    return 0;
}

/* Private functions */
/*************************************************************************/

/* Virtual draw function */
static int _zshafts_draw(void* obj, void* usr_data)
{
    zGeneric* zg;
    Z_CHECK_OBJ(obj);
    zg = (zGeneric*) obj;

    return zShaft_Draw(Z_SHAFT(zg));
}

/* Virtual delete function */
static int _zshafts_delete(void* obj, void* usr_data)
{
    zGeneric* zg;
    Z_CHECK_OBJ(obj);
    zg = (zGeneric*) obj;
    zShaft_Delete(Z_SHAFT(zg));
    return 0;
}
