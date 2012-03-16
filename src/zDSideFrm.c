/* Implementation of Damper drive side frame */

#include "zDSideFrm.h"
#include "zVar.h"

/* Virtual functions */
static int _zdsidefrm_draw(zGeneric* obj);

/* Constructor */
zGeneric* zDSideFrm_New(zDSideFrm* obj)
{
    zGeneric* _gc;
    /* Call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zDSideFrm);

    /* Call parent object */
    _gc = zDFrame_New(&obj->z_parent);
    if(!_gc)
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* set properties */
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* Set function and child pointer of parent object */
    obj->z_parent.z_draw_func = _zdsidefrm_draw;
    obj->z_parent.z_child = (void*) obj;

    /* reutur top level parent object */
    return _gc;
}

/* Destructor */
void zDSideFrm_Delete(zDSideFrm* obj)
{
    /* check object */
    Z_CHECK_OBJ_VOID(obj);

    /* Call destructor of parent object */
    zDFrame_Delete(&obj->z_parent);

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* if internally created delete it */
    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zDSideFrm_Draw(zDSideFrm* obj)
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

    /* Device context */
    _dev_c = zGeneric_Get_Dev_Context(_generic);

    /* Check device context pointer */
    Z_CHECK_OBJ(_dev_c);

    /* Save cairo context and translate to new coordinates */
    cairo_save(_dev_c);

    /* Draw drive frame */
    cairo_rectangle(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS(_base->z_y),
		    CONV_TO_POINTS(_base->z_width),
		    CONV_TO_POINTS(_base->z_height));
    cairo_stroke(_dev_c);

    /* Check if return lip flag was enable and draw it */
    if(obj->z_parent.z_rtlip_flg && _base->z_thk > 0.0)
	{
	    _x = _base->z_x + (obj->z_parent.z_side_flg? _base->z_width : 0.0)
		+ (obj->z_parent.z_side_flg? -1 : 1) * _base->z_thk;
	    _y = _base->z_y + _base->z_height;
	    /* set line type to hidden */
	    zGeneric_Set_LintType(_generic, zLTHidden);
	    cairo_move_to(_dev_c,
			  CONV_TO_POINTS(_x),
			  CONV_TO_POINTS(_base->z_y));
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

/***************************************************************************/
/* Private functions */

/* Virtual draw method */
static int _zdsidefrm_draw(zGeneric* obj)
{
    int rt_val;
    zDSideFrm* _sdfrm;
    
    Z_CHECK_OBJ(obj);
    _sdfrm = Z_DSIDE_FRAME(obj);
    rt_val = zDSideFrm_Draw(Z_DSIDE_FRAME(obj));
    if(_sdfrm->z_draw_func)
	return _sdfrm->z_draw_func(obj);
    else
	return rt_val;
}
