/* Implementation of junction boxes
 * Tue Jan 31 13:00:27 GMTST 2012*/

#include "zJB.h"


/* Virtual function */
static int _zjb_draw(zGeneric* obj);


/* Constructor */
zGeneric* zJB_New(zJB* obj,				/* optional NULL pointer */
		  double width,				/* width */
		  double height,			/* height */
		  double depth,				/* depth */
		  double ang)				/* angle */
{
    /* check for object pointer */
    if(obj == NULL)
	{
	    obj = (zJB*) malloc(sizeof(zJB));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create parent object, if failed return NULL */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg = 1)
		free(obj);

	    return NULL;
	}

    /* Set properties */
    obj->z_terms_ext = 1;
    obj->z_glands_ext = 1;

    obj->z_depth = depth;
    obj->z_ang = ang;
    zBase_Set_Width_and_Height(obj->z_parent, width, height);

    obj->z_terms = NULL;
    obj->z_glands = NULL;
    obj->z_obj_sz = sizeof(zJB);
    obj->z_child = NULL;

    /* Assign function pointer of parent object */
    obj->z_parent.z_draw_func = _zjb_draw;

    /* Set child pointer of parent object */
    obj->z_parent.z_child = (void*) obj;

    /* Return parent pointer */
    return &obj->z_parent.z_generic;
}

/* Destructor */
void zJB_Delete(zJB* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call parent destructor */
    zBase_Delete(obj->z_parent);

    /* Call destructor of terminal collection
     * if internally set */
    if(obj->z_terms_ext == 0)
	zTerminals_Delete(Z_TERMINALS(obj->z_terms));

    /* Destroy glands here */


    obj->z_terms = NULL;
    obj->z_glands = NULL;
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    if(obj->z_int_flg)
	free(obj);
    
}
