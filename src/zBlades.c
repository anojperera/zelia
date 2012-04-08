/* Implementation of Blade collection */
/* Wed Mar 28 21:33:53 BST 2012 */

#include "zBlades.h"

/* Virtual functions */
static int _zblades_draw(void* obj, void* usr_data);
static int _zblades_delete(void* obj, void* usr_data);

/* Constructor */
zGenerics* zBlades_New(zBlades* obj,		/* optional object */
		       zDevice* dev,		/* device pointer */
		       double x,			/* x coordinate of blade collection */
		       double y,			/* y coordinate of blade collection */
		       double width,
		       double height,
		       unsigned int num_blades,
		       zBladeType ztype)
{
    int i;
    double bld_height;
    double bld_y;
    /* constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zBlades);

    /* create parent object */
    if(zGenerics_New(&obj->z_parent,
		     0,
		     num_blades))
	{
	    /* If the object was created internally, delete it */
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set device */
    zGenerics_Set_Device(&obj->z_parent, dev);

    obj->z_x = x;
    obj->z_y = y;
    
    bld_height = height / (double) num_blades;

    for(i=0,bld_y=y; i<num_blades; i++,bld_y += bld_height/2)
	{
	    obj->z_parent.z_generics_s[i] =
		zBlade_New(NULL);

	    /* set blade type */
	    zBlade_Set_Blade_Type(Z_BLADE(obj->z_parent.z_generics_s[i]),
				  ztype);

	    /* top, middle or last blade */
	    if(ztype == zBlade_ISO && i==0 && num_blades > 1)
		{
		    zBlade_Set_Blade_ISO_Type(Z_BLADE(obj->z_parent.z_generics_s[i]),
					      zBladeISO_AB);
		}
	    else if(ztype == zBlade_ISO && i==num_blades-1 && num_blades > 1)
		{
		    zBlade_Set_Blade_ISO_Type(Z_BLADE(obj->z_parent.z_generics_s[i]),
					      zBladeISO_AB);
		}
	    else if(ztype == zBlade_ISO && num_blades==1)
		{
		    zBlade_Set_Blade_ISO_Type(Z_BLADE(obj->z_parent.z_generics_s[i]),
					      zBladeISO_AA);
		}
	    else if(ztype == zBlade_ISO)
		{
		    zBlade_Set_Blade_ISO_Type(Z_BLADE(obj->z_parent.z_generics_s[i]),
					      zBladeISO_BB);
		}

	    /* Set blade x and y coordinates */
	    zBase_Set_Base_Coords(Z_BASE(obj->z_parent.z_generics_s[i]),
				  x,
				  bld_y);

	    /* Set blade width and height */
	    zBase_Set_Width_and_Height(Z_BASE(obj->z_parent.z_generics_s[i]),
				       width,
				       bld_height);
	}

    /* set function pointers of parent object */
    obj->z_parent.z_draw_func = _zblades_draw;
    obj->z_parent.z_destructor_func = _zblades_delete;
    obj->z_parent.z_child = (void*) obj;
    obj->z_parent.z_usr_data = (void*) obj;

    obj->z_obj_sz = sizeof(zBlades);

    /* return parent object */
    return &obj->z_parent;
}


/* Destructor */
void zBlades_Delete(zBlades* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call delete method of parent object */
    zGenerics_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    if(obj->z_int_flg)
	free(obj);
}

/* Private functions */
/*************************************************************************/

/* Virtual draw function */
static int _zblades_draw(void* obj, void* usr_data)
{
    Z_CHECK_OBJ(obj);
    return zBlade_Draw(Z_BLADE(obj));
}

/* Virtual delete function */
static int _zblades_delete(void* obj, void* usr_data)
{
    Z_CHECK_OBJ(obj);
    zBlade_Delete(Z_BLADE(obj));
    return 0;
}
