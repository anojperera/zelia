/* Implementation of blade class */
/* Wed Mar 21 21:17:57 GMT 2012 */

#include "zBlade.h"

/* Virtual function */
static int _zblade_draw(zGeneric* obj);

/* Private drawing functions - draws various types of blades */
static inline int _zblade_draw_iso(zBlade* obj);
static inline int _zblade_draw_niso(zBlade* obj);
static inline int _zblade_draw_lfd(zBlade* obj);
static inline int _zblade_draw_total(zBlade* obj);
static inline int _zblade_draw_brd(zBlade* obj);
static inline int _zblade_draw_prd(zBlade* obj);

/* Constructor */
zGeneric* zBlade_New(zBlade* obj)
{
    /* call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zBlade);

    /* create base object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    obj->z_blade_type = zBlade_ISO;
    obj->z_draw_func = NULL;
    obj->z_child = NULL;
    obj->z_iso_type = zBladeISO_AA;

    /* set child and function pointers of parent object */
    obj->z_parent.z_child = (void*) obj;
    obj->z_parent.z_draw_func = _zblade_draw;

    /* return top level object */
    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zBlade_Delete(zBlade* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* delete parent object */
    zBase_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* if internally created delete it */
    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zBlade_Draw(zBlade* obj)
{
    int rt_val;
    Z_CHECK_OBJ(obj);

    /* call draw functions as per set blade */
    switch(obj->z_blade_type)
	{
	case zBlade_NISO:
	    rt_val = _zblade_draw_niso(obj);
	    break;
	case zBlade_LFD:
	    rt_val = _zblade_draw_lfd(obj);
	    break;
	case zBlade_TOTAL:
	    rt_val = _zblade_draw_total(obj);
	    break;
	case zBlade_BRD:
	    rt_val = _zblade_draw_brd(obj);
	    break;
	case zBlade_PRD:
	    rt_val = _zblade_draw_prd(obj);
	    break;
	default:
	    rt_val = _zblade_draw_iso(obj);
	}

    return rt_val;
}

/*************************************************************************/
/* Property methods */

/* Set blade method */
inline int zBlade_Set_Blade_Type(zBlade* obj, zBladeType ztype)
{
    Z_CHECK_OBJ(obj);
    obj->z_blade_type = ztype;
    return 0;
}

/* Get blade type */
inline zBladeType zBlade_Get_Blade_Type(zBlade* obj)
{
    if(obj == NULL)
	return 0;

    return obj->z_blade_type;
}

/* Set ISO blade type */
inline int zBlade_Set_Blade_ISO_Type(zBlade* obj, zBladeISO_Type ztype)
{
    Z_CHECK_OBJ(obj);
    obj->z_iso_type = ztype;
    return 0;
}

/* Get ISO blade type */
inline zBladeISO_Type zBlade_Get_Blade_ISO_Type(zBlade* obj)
{
    if(obj == NULL)
	return zBladeISO_AA;
    else
	return obj->z_iso_type;
}
/*************************************************************************/
/* Private method */
/* Virtual draw method */
static int _zblade_draw(zGeneric* obj)
{
    int rt_val;
    zBlade* _bld;
    
    Z_CHECK_OBJ(obj);
    _bld = Z_BLADE(obj);
    rt_val = zBlade_Draw(Z_BLADE(obj));
    if(_bld->z_draw_func)
	return _bld->z_draw_func(obj);
    else
	return rt_val;
}

/* Draw isolating blade */
static inline int _zblade_draw_iso(zBlade* obj)
{
    /* object not require checking */
    zBase* _base;			/* base object */
    cairo_t* _dev_c;			/* device context */

    _base = &obj->z_parent;
    _dev_c = zGeneric_Get_Dev_Context(&obj->z_parent.z_sgeneric);

    /* Check width and height */
    if(_base->z_width <= 0.0 || _base->z_height <= 0.0)
	return 1;

    /* draw blade */
    switch(obj->z_iso_type)
	{
	case zBladeISO_AA:
	    cairo_move_to(_dev_c,
	    		  CONV_TO_POINTS(_base->z_x),
	    		  CONV_TO_POINTS((_base->z_y - _base->z_height / 2)));
	    cairo_line_to(_dev_c,
	    		  CONV_TO_POINTS(_base->z_x),
	    		  CONV_TO_POINTS((_base->z_y + _base->z_height / 2)));
	    cairo_move_to(_dev_c,
	    		  CONV_TO_POINTS((_base->z_x + _base->z_width)),
	    		  CONV_TO_POINTS((_base->z_y - _base->z_height / 2)));
	    cairo_line_to(_dev_c,
	    		  CONV_TO_POINTS((_base->z_x + _base->z_width)),
	    		  CONV_TO_POINTS((_base->z_y + _base->z_height / 2)));

	    break;

	case zBladeISO_AB:
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_base->z_x),
			  CONV_TO_POINTS((_base->z_y - _base->z_height / 2)));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_base->z_x),
			  CONV_TO_POINTS((_base->z_y + _base->z_height / 2)));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS((_base->z_x + _base->z_width)),
			  CONV_TO_POINTS((_base->z_y + _base->z_height / 2)));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS((_base->z_x + _base->z_width)),
			  CONV_TO_POINTS((_base->z_y - _base->z_height / 2)));
	    break;
	default:
	    cairo_rectangle(_dev_c,
			    CONV_TO_POINTS(_base->z_x),
			    CONV_TO_POINTS((_base->z_y - _base->z_height / 2)),
			    CONV_TO_POINTS(_base->z_width),
			    CONV_TO_POINTS(_base->z_height));
	}

    /* Fold line */
    cairo_move_to(_dev_c,
		  CONV_TO_POINTS(_base->z_x),
		  CONV_TO_POINTS((_base->z_y - _base->z_height * Z_BLADE_ISO_FOLD)));
    cairo_line_to(_dev_c,
		  CONV_TO_POINTS((_base->z_x + _base->z_width)),
		  CONV_TO_POINTS((_base->z_y - _base->z_height * Z_BLADE_ISO_FOLD)));

    cairo_move_to(_dev_c,
		  CONV_TO_POINTS(_base->z_x),
		  CONV_TO_POINTS((_base->z_y + _base->z_height * Z_BLADE_ISO_FOLD)));
    cairo_line_to(_dev_c,
		  CONV_TO_POINTS((_base->z_x + _base->z_width)),
		  CONV_TO_POINTS((_base->z_y + _base->z_height * Z_BLADE_ISO_FOLD)));
    cairo_stroke(_dev_c);

    _base = NULL;
    _dev_c = NULL;
    
    return 0;
}

/* Draw non isolating blade */
static inline int _zblade_draw_niso(zBlade* obj)
{
    return 0;
}

/* Draw LFD blade */
static inline int _zblade_draw_lfd(zBlade* obj)
{
    return 0;
}

/* Draw total blade */
static inline int _zblade_draw_total(zBlade* obj)
{
    return 0;
}

/* draw brd blade */
static inline int _zblade_draw_brd(zBlade* obj)
{
    return 0;
}

/* draw prd blade */
static inline int _zblade_draw_prd(zBlade* obj)
{
    return 0;
}
