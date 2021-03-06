/* Implementation of Cell class of table */
/* Sat Mar  3 13:16:04 GMT 2012 */

#include <string.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>
#include "ztcell.h"

#define ZTCELL_TEXT_LEFT 2.4
#define ZTCELL_TEXT_TOP 1.5

/* Virtual functions */
static int _ztcell_draw(void* obj);
static int _ztcell_delete(void* obj);

/* Constructor */
zgeneric* ztcell_new(ztcell* obj)
{
    /* call constructor helper */
    ZCONSTRUCTOR(obj, ztcell);

    /* call parent constructor */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->ref_flg = 0;

    obj->_row_ix = 0;
    obj->_col_ix = 0;
    obj->child = NULL;
    obj->content_sz = 0;    
    obj->content = NULL;

    obj->line_flg = 1;

    /* set parent objects function pointers */
    zgeneric_set_draw(obj, _ztcell_draw);
    zgeneric_set_delete_callback(obj, _ztcell_delete);
    
    /* set child pointer of parent object */
    zgeneric_set_child_pointer(obj);
    return obj->super_cls;
}

/* Destructor */
void ztcell_delete(ztcell* obj)
{
    ZCHECK_OBJ_VOID(obj);

    /* delete parent object */
    zgeneric_block_parent_destructor(obj);
    zbase_delete(&obj->parent);

    if(obj->content && obj->content_sz > 0)
	free(obj->content);
    obj->content = NULL;
    obj->ref_flg = 0;
    obj->content_sz = 0;
    obj->child = NULL;
    obj->super_cls = NULL;

    /* If object was internally created,
     * destroy it */
    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Draw function */
int ztcell_draw(ztcell* obj)
{
    cairo_t* _dev_c;
    zbase* _base;
    PangoLayout* _layout;
    PangoFontDescription* _desc;
    double _x, _y;

    /* check object */
    ZCHECK_OBJ_INT(obj);

    /* Get base and generic object */
    _base = &obj->parent;

    /* Check object */
    ZCHECK_OBJ_INT(_base);
    ZCHECK_OBJ_INT(obj->super_cls);

    /* Get cairo device context */
    _dev_c = zgeneric_get_dev_context(obj->super_cls);
    ZCHECK_OBJ_INT(_dev_c);

    /* Check width and height */
    if(_base->width <= 0.0 &&
       _base->height <= 0.0)
	return ZELIA_TCELL_ERROR;

    /* Save context */
    cairo_save(_dev_c);

    /* Draw rectangle */
    if(obj->line_flg)
	{
	    cairo_save(_dev_c);
	    cairo_rectangle(_dev_c,
	    		    ZCONV_TO_POINTS(_base->x),
	    		    ZCONV_TO_POINTS(_base->y),
	    		    ZCONV_TO_POINTS(_base->width),
	    		    ZCONV_TO_POINTS(_base->height));
	    cairo_stroke(_dev_c);
	    cairo_restore(_dev_c);
	}

    if(obj->content != NULL)
    	{
    	    /* Translate to coordinates */
    	    _x = _base->x + ZTCELL_TEXT_LEFT;
    	    _y = _base->y + ZTCELL_TEXT_TOP;

    	    cairo_translate(_dev_c, ZCONV_TO_POINTS(_x), ZCONV_TO_POINTS(_y));

    	    /* Create pango layout */
    	    _layout = pango_cairo_create_layout(_dev_c);

    	    /* Add text to layout */
    	    pango_layout_set_text(_layout, obj->content, -1);

    	    /* Create font description and add to layout */
    	    _desc = pango_font_description_from_string(Z_GRD_FONT_STYLE);
    	    pango_layout_set_font_description(_layout, _desc);
    	    pango_font_description_free(_desc);

    	    pango_cairo_show_layout(_dev_c, _layout);

    	    /* Free layout object */
    	    g_object_unref(_layout);
    	}

    /* Restore cairo context */
    cairo_restore(_dev_c);

    _dev_c = NULL;
    _base = NULL;
    _layout = NULL;
    _desc = NULL;

    return ZELIA_OK;
}

/*=================================== Private Methods ===================================*/
/* Virtual draw function */
static int _ztcell_draw(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    return ztcell_draw(Z_TCELL(_zg));
}

static int _ztcell_delete(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    ztcell_delete(Z_TCELL(_zg));
    return ZELIA_OK;
}
