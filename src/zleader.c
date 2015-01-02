/* Implementation of zLeader class
 * Tue Feb  7 16:11:53 GMTST 2012 */

#include <string.h>
#include <math.h>
#include <pango/pangocairo.h>
#include "zleader.h"

#define ZLEADER_ARR 3

/* Virtual draw function */
static int _zleader_draw(void* obj);
static int _zleader_delete(void* obj);

/* Constructor */
zgeneric* zleader_new(zleader* obj,				/* leader object optional */
		      zdevice* dev,				/* device object optional */
		      double x,				        /* base coordinate x */
		      double y,					/* base coordinate y */
		      double seg_1,				/* first segment length */
		      double seg_2,				/* second segment length */
		      double ang,				/* orientation angle */
		      const char* desc)				/* description */
{

    /* check for object */
    ZCONSTRUCTOR(obj, zleader);

    /* create parent object */
    if(!(obj->super_cls = zarrow_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);	    
	    return NULL;
	}

    /* if dev object is set make top level default */
    if(dev)
	{
	    zgeneric_set_device(obj->super_cls, dev);
	    zgeneric_set_default_dev_context(obj->super_cls);
	}

    /* set properties */
    zbase_set_base_coords(Z_BASE(obj->super_cls), x, y);
    zbase_set_orientation(Z_BASE(obj->super_cls), ang);
    obj->length_1 = seg_1;
    obj->length_2 = seg_2;

    if(desc)
	strcpy(obj->content, desc);
    else
	obj->content[0] = '\0';

    obj->child = NULL;

    /* set function pointer of parent object */
    zgeneric_set_draw(obj, _zleader_draw);
    zgeneric_set_delete_callback(obj, _zleader_delete);

    /* set child pointer of parent object */
    zgeneric_set_child_pointer(obj);

    return obj->super_cls;
}

/* Destructor */
void zleader_delete(zleader* obj)
{
    ZCHECK_OBJ_VOID(obj);

    ZGENERIC_INIT_VTABLE(obj);

    if(obj->vtable.zgeneric_delete)
	{
	    obj->vtable.zgeneric_delete((void*) obj->super_cls);
	    return;
	}

    /* call destructor of base object */
    zgeneric_block_parent_destructor(obj);
    zarrow_delete(&obj->parent);
    
    obj->child = NULL;
    return;
}

/* Draw method */
int zleader_draw(zleader* obj)
{
    zbase* _base = NULL;
    cairo_t* _dev_c = NULL;
    double x[ZLEADER_ARR], y[ZLEADER_ARR], ang;
    PangoLayout* _layout;				/* pango layout */
    PangoFontDescription* _desc;			/* font description */

    /* check for object */
    ZCHECK_OBJ_INT(obj);

    _base = Z_BASE(obj->super_cls);

    /* Device context */
    _dev_c = zgeneric_get_dev_context(obj->super_cls);

    /* Check all objects */
    ZCHECK_OBJ_INT(_dev_c);

    ZELIA_LOG_MESSAGE("zleader start drawing");
    
    /* Check if the angle is greater than zero.
     * If the angle is greater than zero, save cairo context
     * and translate and rotate the coordinate system */
    cairo_save(_dev_c);

    cairo_translate(_dev_c,
		    ZCONV_TO_POINTS(_base->x),
		    ZCONV_TO_POINTS(_base->y));

    if(_base->ang > 0.0)
	{
	    cairo_rotate(_dev_c,
			 ZCONV_TO_RADIANS(_base->ang));
	}

    cairo_translate(_dev_c,
		    0.0,
		    -1 * ZCONV_TO_POINTS(1));

    ang = 90.0 - _base->ang;
    x[0] = 0.0;
    y[0] = 0.0;

    x[1] = 0.0;
    y[1] = -1 * obj->length_1;

    x[2] = obj->length_2 * sin(ZCONV_TO_RADIANS(ang));
    y[2] = y[1] + -1 * obj->length_2 * cos(ZCONV_TO_RADIANS(ang));

    cairo_move_to(_dev_c,
		  ZCONV_TO_POINTS(x[0]),
		  ZCONV_TO_POINTS(y[0]));

    cairo_line_to(_dev_c,
		  ZCONV_TO_POINTS(x[1]),
		  ZCONV_TO_POINTS(y[1]));

    cairo_line_to(_dev_c,
		  ZCONV_TO_POINTS(x[2]),
		  ZCONV_TO_POINTS(y[2]));

    cairo_stroke(_dev_c);

    /* Add description if set */
    while(obj->content[0] != '\0')
	{

	    /* create pango layout */
	    _layout = pango_cairo_create_layout(_dev_c);
	    _desc = pango_font_description_from_string(Z_GRD_FONT_STYLE);
	    pango_font_description_set_style(_desc, PANGO_STYLE_NORMAL);
	    pango_font_description_set_size(_desc, PANGO_SCALE * Z_GRD_FONT_SZ);
	    pango_font_description_set_weight(_desc, PANGO_WEIGHT_LIGHT);

	    pango_layout_set_font_description(_layout, _desc);
	    pango_layout_set_text(_layout, obj->content, -1);


	    cairo_translate(_dev_c,
	    		    ZCONV_TO_POINTS(x[1]),
	    		    ZCONV_TO_POINTS(y[1]) - 1.8 * Z_GRD_FONT_SZ);

	    cairo_rotate(_dev_c, -1 * ZCONV_TO_RADIANS(_base->ang));

	    pango_cairo_update_layout(_dev_c, _layout);
	    pango_cairo_show_layout(_dev_c, _layout);

	    pango_font_description_free(_desc);

	    /* free layout object */
	    g_object_unref(_layout);

	    _desc = NULL;
	    _layout = NULL;
	    break;
	}

    cairo_restore(_dev_c);

    ZELIA_LOG_MESSAGE("zleader compelete drawing");
    return ZELIA_OK;

}

/*===================================================================================*/
/* Virtual function */

/* Virtual draw method */
static int _zleader_draw(void* obj)
{
    zgeneric* _zg = NULL;
    zleader* _zl = NULL;
    int rt_val;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    rt_val = zleader_draw(Z_LEADER(_zg));
    _zl = Z_LEADER(_zg);

    if(_zl->vtable.zgeneric_draw)
	return _zl->vtable.zgeneric_draw(obj);
    else
	return rt_val;
}


static int _zleader_delete(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    zleader_delete(Z_LEADER(_zg));
    return  ZELIA_OK;
}
