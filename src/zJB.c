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
    zBase_Set_Width_and_Height(&obj->z_parent, width, height);

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

/* Draw function */
int zJB_Draw(zJB* obj)
{
    zBase* _base;				/* base object pointer */
    zGeneric* _genric;				/* generic object pointer */
    cairo_t* _dev_c;				/* cairo context */
    
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    _base = &obj->z_parent;
    _genric = &obj->z_parent.z_sgeneric;

    /* Device context pointer */
    _dev_c = zGeneric_Get_Dev_Context(_genric);

    /* Check all objects */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_genric);
    Z_CHECK_OBJ(_dev_c);

    /* move device context to the base coordinates and
     * draw outside rectangle */
    cairo_rectangle(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS(_base->z_y),
		    CONV_TO_POINTS(_base->z_width),
		    CONV_TO_POINTS(_base->z_height));

    /* call stroke as terminals and glands may have not assigned */
    cairo_stroke(_dev_c);

    /* call draw functions of glands and terminals */
    zGenerics_Draw(obj->z_terms);
    zGenerics_Draw(obj->z_glands);

    /* call child drawing function pointer if assigned */
    if(obj->z_draw_func)
	return obj->z_draw_func(_genric);

    return 0;
}

/* Set external terminal collection */
inline int zJB_Set_Terminals(zJB* obj, zGenerics* terms)
{
    /* check for objects */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(terms);

    obj->z_terms = terms;

    /* indicate external flag was set */
    obj->z_terms_ext = 1;
    return 0;
}

/* Get terminals collection */
inline zGenerics* zJB_Get_Terminals(zJB* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_terms;
}
