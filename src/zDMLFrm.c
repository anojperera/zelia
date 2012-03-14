/* Implementation of mullion frame */
/* Wed Mar 14 16:52:19 GMTST 2012 */

#include "zDMLFrm.h"
#include "zVar.h"

/* Virtual functions */
static int _zdmlfrm_draw(zGeneric* obj);

/* Constructor */
zGeneric* zDMLFrm_New(zDMLFrm* obj)
{
    zGeneric* _zg;
    
    /* call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zDMLFrm);

    /* create parent object */
    _zg = zDFrame_New(&obj->z_parent);
    if(!_zg)
	{
	    if(obj->z_int_flg)
		free(obj);
	    return;
	}

    /* set properties */
    obj->z_ml_type = 0;
    obj->z_ml_return_lip = Z_MULLION_RETURN_LIP;
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* Set function and child pointer of parent object */
    obj->z_parent.z_draw_func = _zdmlfrm_draw;
    obj->z_parent.z_child = (void*) obj;

    /* return top level class */
    return _zg;
}

/* Destructor */
void zDMLFrm_Delete(zDMLFrm* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* call destructor of parent object */
    zDFrame_Delete(&obj->z_parent);

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* if object internally created destroy */
    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zDMLFrm_Draw(zDMLFrm* obj)
{
    zGeneric* _generic;
    zBase* _base;
    cairo_t* _dev_c;
    double _x, _y;

    /* check object */
    Z_CHECK_OBJ(obj);

    /* get base and generic object pointer */
    _base = &obj->z_parent.z_parent;
    _generic = &obj->z_parent.z_parent.z_sgeneric;

    /* Get device context */
    _dev_c = zGeneric_Get_Dev_Context(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* Save cairo context */
    cairo_save(_dev_c);

    /* Draw mullion */
    cairo_rectangle(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS(_base->z_y),
		    CONV_TO_POINTS(_base->z_width),
		    CONV_TO_POINTS(_base->z_height));
    cairo_stroke(_dev_c);
    
    _y = _base->z_y + obj->z_height;
    if(obj->z_ml_type == 0)
	{
	    _x = _base->z_x + Z_MULLION_COVERPLATE_GAP;
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_base->z_y));
	    
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    _x = _base->z_x + _base->z_width - Z_MULLION_COVERPLATE_GAP;
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_base->z_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    cairo_stroke(_dev_c);
	}

    /* draw hidden lines */
    if(obj->z_ml_return_lip > 0.0)
	{
	    zGeneric_Set_LintType(_generic, zLTHidden);
	    _x = _base->z_x + obj->z_ml_return_lip;
	    cairo_mov_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_base->z_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    _x = _base->z_x + _base->z_width - obj->z_ml_return_lip;
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_base->z_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    cairo_stroke(_dev_c);
	    zGeneric_Set_LintType(_generic, zLTContinuous);
	}

    /* restor cairo context */
    cairo_restore(_dev_c);

    _generic = NULL;
    _base = NULL;
    _dev_c = NULL;
    return 0;
}

/*************************************************************************************/
/* Property methods */

/* Set mullion type */
inline int zDMLFrm_Set_Type(zDMLFrm* obj, unsigned int ztype)
{
    Z_CHECK_OBJ(obj);
    obj->z_ml_type = ztype;
    return 0;
}

/* Get mullion type */
inline unsigned int zDMLFrm_Get_Type(zDMLFrm* obj)
{
    if(obj == NULL)
	return 0;
    return obj->z_ml_type;
}

/* Set mullion return lip */
inline int zDMLFrm_Set_Return_Lip(zDMLFrm* obj, double width)
{
    Z_CHECK_OBJ(obj);
    obj->z_ml_return_lip = width;
    return 0;
}

/* Get mullion return lip width */
inline double zDMLFrm_Get_Return_Lip(zDMLFrm* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_ml_return_lip;
}

/************************************************************************************************************/
/* Private mathods */

/* Virtual draw method */
static int _zdmlfrm_draw(zGeneric* obj)
{
    zDMLFrm* _dml;
    int rt_val;
    Z_CHECK_OBJ(obj);
    _dml = Z_DML_FRAME(obj);
    rt_val = zDMLFrm_Draw(Z_DML_FRAME(obj));
    if(_dml->z_draw_func)
	return _dml->z_draw_func(obj);
    else
	return rt_val;
    
}
