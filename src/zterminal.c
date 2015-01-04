/* Implementation of Terminal class
 * Sat Jan 21 18:06:55 GMT 2012 */

#include <stdio.h>
#include "zterminal.h"

/* Virtual draw function */
static int _zterminal_draw_function(void* obj);
static int _zterminal_delete(void* obj);

/* Constructor */
zgeneric* zterminal_new(zterminal* obj)
{

    ZCONSTRUCTOR(obj, zterminal);

    /* Create base object */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);

	    return NULL;
	}

    /* set properties */
    obj->term_type = zSAK2pt5;
    obj->term_height = 0.0;

    obj->term_cnt = 1;					/* set terminal count to 1 */
    obj->term_num[0] = '1';				/* set terminal number to 1 */
    obj->term_num[1] = '\0';

    /* initialise the vtable */
    ZGENERIC_INIT_VTABLE(obj);

    /* set drawing pointer */
    zgeneric_set_draw(obj, _zterminal_draw_function);
    zgeneric_set_delete_callback(obj, _zterminal_delete);

    /* set child pointer */
    zgeneric_set_child_pointer(obj);

    /* Return base pointer */
    return obj->super_cls;
}

/* Destructor */
void zterminal_delete(zterminal* obj)
{
    ZCHECK_OBJ_VOID(obj);

    if(obj->vtable.zgeneric_delete)
	{
	    obj->vtable.zgeneric_delete((void*) obj->super_cls);
	    return;
	}

    /* delete parent object */
    zgeneric_block_parent_destructor(obj);
    zbase_delete(&obj->parent);


    obj->child = NULL;
    obj->super_cls = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Draw function */
int zterminal_draw(zterminal* obj)
{
    zbase* _base;
    cairo_t* _dev_c;
    double x, y;
    cairo_text_extents_t _te;
    
    /* Check for object */
    ZCHECK_OBJ_INT(obj);

    /* Get base and generic object */
    _base = &obj->parent;

    /* Device context pointer */
    _dev_c = zgeneric_get_dev_context(obj->super_cls);

    /* Check all objects */
    ZCHECK_OBJ_INT(_base);
    ZCHECK_OBJ_INT(obj->super_cls);
    ZCHECK_OBJ_INT(_dev_c);

    /* move device context to base coordinates */
    cairo_rectangle(_dev_c,
		    ZCONV_TO_POINTS(_base->x),
		    ZCONV_TO_POINTS(_base->y),
		    ZCONV_TO_POINTS(_base->width),
		    ZCONV_TO_POINTS(_base->height));

    /* cairo stroke to draw */
    cairo_stroke(_dev_c);

    /* add terminal number */
    cairo_select_font_face(_dev_c,
			   Z_GRD_FONT_STYLE,
			   CAIRO_FONT_SLANT_NORMAL,
			   CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(_dev_c, Z_GRD_FONT_SZ);

    cairo_text_extents(_dev_c,
		       obj->term_num,
		       &_te);
    x = _base->x + (_base->width - ZCONV_TO_MM(_te.width)) / 2 - ZCONV_TO_MM(_te.x_bearing);
    y = _base->y + _base->height / 2;
    cairo_move_to(_dev_c,
		  ZCONV_TO_POINTS(x),
		  ZCONV_TO_POINTS(y));
    cairo_show_text(_dev_c, obj->term_num);

    return ZELIA_OK;
}


/*=====================================================================*/
/* Virtual function implementation */
static int _zterminal_draw_function(void* obj)
{
    zgeneric* _zg;
    zterminal* _self;

    int _rt = ZELIA_OK;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    _self = Z_TERMINAL(_zg);

    /* call draw function */
    _rt = zterminal_draw(_self);

    /* Check for child function pointer and call */
    if(_self->vtable.zgeneric_draw)
	return _self->vtable.zgeneric_draw(obj);
    else
	return _rt;
}

static int _zterminal_delete(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    zterminal_delete(Z_TERMINAL(_zg));
    return ZELIA_OK;
}
