/* Implementation of bearing class */
/* Tue Apr 24 23:04:34 BST 2012 */

#include "zBearing.h"
#include <math.h>

/* Virtual draw function */
static int _zbearing_draw(zGeneric* obj);

/* private methods */
static inline int _zbearing_draw_cutdown(zBearing* obj);		/* draw cutdown bearing */
static inline int _zbearing_draw_blind(zBearing* obj);			/* draw blind bearing */
static inline int _zbearing_draw_linkage(zBearing* obj);		/* linkage bearing */

/* Constructor */
zGeneric* zBearing_New(zBearing* obj)
{
    /* call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zBearing);

    /* Create base object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    obj->z_type = zBearingCutdown;		/* Cutdown bearing set by default */
    obj->z_child = NULL;
    obj->z_draw_func = NULL;
    obj->z_obj_sz = sizeof(zBearing);
    
    /* Set child and function pointers of parent object */
    obj->z_parent.z_child = (void*) obj;
    obj->z_parent.z_draw_func = _zbearing_draw;

    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zBearing_Delete(zBearing* obj)
{
    /* check for object */
    Z_CHECK_OBJ_VOID(obj);

    /* call destructor of parent object */
    zBase_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* if class was internally created delete it */
    if(obj->z_int_flg)
	free(obj);
    
}

/* Draw method */
int zBearing_Draw(zBearing* obj)
{
    /* object check */
    Z_CHECK_OBJ(obj);

    switch(obj->z_type)
	{
	case zBearingBlind:
	    _zbearing_draw_blind(obj);
	    break;
	case zBearingRemoval:
	    _zbearing_draw_linkage(obj);
	    break;
	default:
	    _zbearing_draw_cutdown(obj);
	}
    return 0;
}

/* Set bearing type */
inline int zBearing_Set_Type(zBearing* obj, zBearingTypes ztype)
{
    /* check for object */
    Z_CHECK_OBJ(obj);
    obj->z_type = ztype;
    return 0;
}

/* Get bearing type */
inline zBearingTypes zBearing_Get_Type(zBearing* obj)
{
    if(obj == NULL)
	return zBearingCutdown;
    else
	return obj->z_type;
}

/****************************************************************************/
/* Private functions */

/* Virtual draw method */
static int _zbearing_draw(zGeneric* obj)
{
    zBearing* _bar;
    int rt_val = 0;
    /* object check */
    Z_CHECK_OBJ(obj);
    _bar = Z_BEARING(obj);
    rt_val = zBearing_Draw(_bar);
    if(_bar->z_draw_func)
	return _bar->z_draw_func(obj);
    else
	return rt_val;
}

/* Draw cutdown bearing */
static inline int _zbearing_draw_cutdown(zBearing* obj)
{
    zGeneric* _generic;
    zBase* _base;
    cairo_t* _dev_c;
    double _height;
    /* check for object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    _base = &obj->z_parent;
    _generic = &obj->z_parent.z_sgeneric;

    _dev_c = zGeneric_Get_Dev_Context(_generic);

    /* check all objects */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* save cairo context */
    cairo_save(_dev_c);

    /* change line type to hidden */
    zGeneric_Set_LineType(_generic, zLTHidden);
    cairo_rectangle(_dev_c,
		    _base->z_x,
		    _base->z_y - _base->z_height / 2;
		    Z_BEARING_CT_FLANGE_THICKNESS,
		    _base->z_height);
    _height = _base->z_height * Z_BEARING_CT_BASE_HEIGHT_FCT;
    cairo_rectangle(_dev_c
		    _base->z_x + Z_BEARING_CT_FLANGE_THICKNESS,
		    _base->z_y - _height / 2,
		    Z_BEARING_CT_BASE_THICKNESS,
		    _height);
    cairo_stroke(_dev_c);
    zGeneric_Set_LintType(_generic, zLTContinuous);

    cairo_retore(_dev_c);
    return 0;
}
