/* Implementation of shaft class */
/* Tue Apr 10 21:55:02 BST 2012 */

#include "zShaft.h"

/* Virtual functions */
static int _zshaft_draw(zGeneric* obj);

/* Constructor */
zGeneric* zShaft_New(zShaft* obj)
{
    /* Constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zShaft);

    /* Create base object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* assign function pointers of parent object */
    obj->z_parent.z_draw_func = _zshaft_draw;
    obj->z_parent.z_child = (void*) obj;
    obj->z_obj_sz = sizeof(zShaft);
    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zShaft_Delete(zShaft* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call to delete base object */
    zBase_Delete(&obj->z_parent);

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    if(obj->z_int_flg)
	free(obj);
    return;
}


/* Draw method */
int zShaft_Draw(zShaft* obj)
{
    zGeneric* _generic;
    zBase* _base;
    cairo_t* _dev_c;

    /* Check object */
    Z_CHECK_OBJ(obj);

    _base = &obj->z_parent;
    _generic = &obj->z_parent.z_sgeneric;

    /* Get cairo device context */
    _dev_c = zGeneric_Get_Dev_Context(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* Save cairo context */
    cairo_save(_dev_c);

    /* Change line type to hidden */
    zGeneric_Set_LintType(_generic, zLTHidden);
    cairo_rectangle(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS((_base->z_y - _base->z_height / 2)),
		    CONV_TO_POINTS(_base->z_width),
		    CONV_TO_POINTS(_base->z_height));
    cairo_stroke(_dev_c);
    zGeneric_Set_LintType(_generic, zLTContinuous);

    cairo_restore(_dev_c);

    _generic = NULL;
    _base = NULL;
    _dev_c = NULL;

    return 0;
}

/****************************************************************************/
/* Private functions */

/* Draw method */
static int _zshaft_draw(zGeneric* obj)
{
    zShaft* _shft;
    int rt_val;
    
    Z_CHECK_OBJ(obj);
    rt_val = zShaft_Draw(Z_SHAFT(obj));
    _shft = Z_SHAFT(obj);
    if(_shft->z_draw_func)
	return _shft->z_draw_func(obj);
    else
	return rt_val;
}
