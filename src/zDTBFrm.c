/* Implementation of top and bottom frame */
/* Tue Mar 13 23:13:25 GMT 2012 */

#include "zDTBFrm.h"

/* Virtual functions */
static int _zdtbfrm_draw(zGeneric* obj);

/* Constructor */
zGeneric* zDTBFrm_New(zDTBFrm* obj)
{
    zGeneric* _zg;
    
    /* call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zDTBFrm);

    /* create parent object */
    _zg = zDFrame_New(&obj->z_parent);
    if(!_zg)
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* Set function and child pointer of parent object */
    obj->z_parent.z_draw_func = _zdtbfrm_draw;
    obj->z_parent.z_child = (void*) obj;

    /* return top level parent object */
    return _zg;
}

/* Destructor */
void zDTBFrm_Delete(zDTBFrm* obj)
{
    /* check object */
    Z_CHECK_OBJ_VOID(obj);

    /* call destructor of parent object */
    zDFrame_Delete(&obj->z_parent);

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* if internally created, delete it */
    if(obj->z_int_flg)
	free(obj);
}

int zDTBFrm_Draw(zDTBFrm* obj)
{
    zGeneric* _generic;
    zBase* _base;
    cairo_t* _dev_c;
    double _x, _y;

    /* check object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    _base = &obj->z_parent.z_parent;
    _generic = &obj->z_parent.z_parent.z_sgeneric;

    /* Get Device context and check */
    _dev_c = zGeneric_Get_Dev_Context(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* Save cairo context and translate to new coordinates */
    cairo_save(_dev_c);

    /* Draw frame */
    cairo_rectangle(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS(_base->z_y),
		    CONV_TO_POINTS(_base->z_width),
		    CONV_TO_POINTS(_base->z_height));
    cairo_stroke(_dev_c);
    
    if(obj->z_parent.z_rtlip_flg && _base->z_thk > 0.0)
	{
	    _x = _base->z_x + _base->z_width;
	    _y = _base->z_y + (obj->z_parent.z_side_flg? _base->z_height : 0.0)
		+ (obj->z_parent.z_side_flg? -1 : 1) * _base->z_thk;
	    /* set line type hidden */
	    zGeneric_Set_LintType(_generic, zLTHidden);
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
	    _x = _base->z_x + _base->z_width;
	    _y = _base->z_y + (obj->z_parent.z_side_flg? 2 * _base->z_thk : _base->z_height) - _base->z_thk;

	    /* set line to hidden */
	    zGeneric_Set_LintType(_generic, zLTHidden);
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_base->z_x),
			  CONV_TO_POINTS(_y));
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_y));
	    cairo_stroke(_dev_c);
	    zGeneric_Set_LintType(_generic, zLTContinuous);	    
	}

    /* Restore device context */
    cairo_restore(_dev_c);
    
    _base = NULL;
    _generic = NULL;
    _dev_c = NULL;

    return 0;    
}

/***********************************************************************/
/* Private Functions */

/* Virtual draw method */
static int _zdtbfrm_draw(zGeneric* obj)
{
    zDTBFrm* _dtb;
    int rt_val;
    
    Z_CHECK_OBJ(obj);
    _dtb = Z_DTB_FRAME(obj);
    rt_val = zDTBFrm_Draw(Z_DTB_FRAME(obj));
    if(_dtb->z_draw_func)
	return _dtb->z_draw_func(obj);
    else
	return rt_val;
}
