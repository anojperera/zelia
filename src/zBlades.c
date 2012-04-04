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

    for(i=0; i<num_blades; i++)
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

	    
	}
}
