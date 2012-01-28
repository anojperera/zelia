/* Implemenetation of zTerminal class */
/* Sat Jan 28 20:20:10 GMT 2012 */

#include "zVar.h"
#include "zTerminals.h"
#include "zTerminal.h"
#include "zBase.h"
#include "zGeneric.h"

/* Virtual functions */
static int _zterminals_delete(zGenerics* obj);		/* delte function */
static int _zterminals_draw(zGenerics* obj);		/* draw function */

/* Constructor */
zGenerics* zTerminals_New(zTerminals* obj,
			  zDevice* dev,			/* device object */
			  unsigned int num_term,	/* number of terminals */
			  double x,			/* base coordinate */
			  double y,			/* base coordinate */
			  double width,		/* width */
			  double height,		/* height */
			  double ang,		/* orientation angle */
			  const char* links)	/* links */
{
    int i;
    
    /* check for object */
    if(obj == NULL)
	{
	    obj = (zTerminals*) malloc(sizeof(zTerminals));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* create parent object */
    if(zGenerics_New(&obj->z_parent,
		     0,
		     num_term))
	{
	    /* If the object was created internally, delete it */
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set device */
    zGenerics_Set_Device(&obj->z_parent, dev);
    
    /* set properties */
    obj->z_x = x;
    obj->z_y = y;
    obj->z_width = width;
    obj->z_height = height;
    obj->ang = ang;

    strcpy(obj->z_term_links, links);

    /* create objects */
    for(i=0; i<num_term; i++)
	{
	    obj->z_parent.z_generics_s[i] =
		zTerminal_New(NULL);

	    /* Set terminal number */
	    zTerminal_Set_Terminal_Number(obj->z_parent.z_generics_s[i],
					  i+1);
	    
	    zBase_Set_Base_Coords(Z_BASE(obj->z_parent.z_generics_s[i]),
				  ang==90.0? x : x + (double) i * width,
				  ang==90.0? y + (double) i * width : y);	    
	    zBase_Set_Width_and_Height(Z_BASE(obj->z_parent.z_generics_s[i]),
				       ang==90.0? height : width,
				       ang==90.0? width : height);

	    zGeneric_Set_Device(obj->z_parent.z_generics_s[i],
				zGenerics_Get_Device(&obj->z_parent));
	}

    /* set function pointers of parent object */
    obj->z_parent.z_destructor_func = _zterminals_delete;
    obj->z_parent.z_draw_func = _zterminals_draw;

    /* child pointer of base object */
    obj->z_parent.z_child = (void*) obj;

    /* return parent object */
    return &obj->z_parent;
    
}
