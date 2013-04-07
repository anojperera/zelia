/* Implementation of Transom frame */
/* Wed Mar 14 17:47:11 GMTST 2012 */

#include "zDTRFrm.h"
#include "zVar.h"

/* Virtual function */
static int _zdtrfrm_draw(zGeneric* obj);

/* Constructor */
zGeneric* zDTRFrm_New(zDTRFrm* obj)
{
    zGeneric* _zg;
    
    /* call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zDTRFrm);

    /* create parent object */
    _zg = zDFrame_New(&obj->z_parent);
    if(!_zg)
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    obj->z_tr_return_lip = Z_TRANSOM_RETURN_LIP;
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* Set child and function pointer */
    obj->z_parent.z_draw_func = _zdtrfrm_draw;
    obj->z_parent.z_child = (void*) obj;

    return _zg;
}

/* Destructor */
void zDTRFrm_Delete(zDTRFrm* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Delete parent object */
    zDFrame_Delete(&obj->z_parent);

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* if internally created delete it */
    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zDTRFrm_Draw(zDTRFrm* obj)
{
    zGeneric* _generic;
    zBase* _base;
    cairo_t* _dev_c;
    double _x, _y;

    Z_CHECK_OBJ(obj);

    /* get base and generic object pointer */
    _base = &obj->z_parent.z_parent;
    _generic = &obj->z_parent.z_parent.z_sgeneric;

    /* Get device context */
    _dev_c = zGeneric_Get_Dev_Context(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* Save cairo context */
    cairo_save(_dev_c);

    /* Draw transom */
    cairo_rectangle(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS(_base->z_y),
		    CONV_TO_POINTS(_base->z_width),
		    CONV_TO_POINTS(_base->z_height));
    cairo_stroke(_dev_c);

    _x = _base->z_x + _base->z_width;
    if(obj->z_tr_return_lip > 0.0)
	{
	    _y = _base->z_y + obj->z_tr_return_lip;
	    zGeneric_Set_LintType(_generic, zLTHidden);
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_base->z_x),
			  CONV_TO_POINTS(_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    
	    _y = _base->z_y + _base->z_height - obj->z_tr_return_lip;
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_base->z_x),
			  CONV_TO_POINTS(_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    cairo_stroke(_dev_c);
	    zGeneric_Set_LintType(_generic, zLTContinuous);
	}

    if(_base->z_thk > 0.0)
	{
	    _y = _base->z_y + _base->z_thk;
	    zGeneric_Set_LintType(_generic, zLTHidden);
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_base->z_x),
			  CONV_TO_POINTS(_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    _y = _base->z_y + _base->z_height - _base->z_thk;
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_base->z_x),
			  CONV_TO_POINTS(_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    cairo_stroke(_dev_c);
	    zGeneric_Set_LintType(_generic, zLTContinuous);
	    
	}

    /* restore cairo */
    cairo_restore(_dev_c);
    _generic = NULL;
    _base = NULL;
    _dev_c = NULL;
    return 0;
}

/* Set transom return lip width */
inline int zDTRFrm_Set_Return_Lip(zDTRFrm* obj, double width)
{
    Z_CHECK_OBJ(obj);
    obj->z_tr_return_lip = width;
    return 0;
}

/* Get transom return lip width */
inline double zDTRFrm_Get_Return_Lip(zDTRFrm* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_tr_return_lip;
}

/* Private Function */
/* Virtual draw function */
static int _zdtrfrm_draw(zGeneric* obj)
{
    zDTRFrm* _tr;
    int rt_val;

    Z_CHECK_OBJ(obj);
    rt_val = zDTRFrm_Draw(Z_DTR_FRAME(obj));
    _tr = Z_DTR_FRAME(obj);
    if(_tr->z_draw_func)
	return _tr->z_draw_func(obj);
    else
	return rt_val;
}
