/* Implementation of Cell class of table */
/* Sat Mar  3 13:16:04 GMT 2012 */

#include "zTCell.h"
#include <string.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

/* Virtual functions */
static int _ztcell_draw(zGeneric* obj);

/* Constructor */
zGeneric* zTCell_New(zTCell* obj)
{
    /* Check for object */
    if(obj == NULL)
	{
	    obj = (zTCell*) malloc(sizeof(zTCell));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create parent object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
	        free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->_z_row_ix = 0;
    obj->_z_col_ix = 0;
    obj->z_child = NULL;
    obj->z_draw_func = NULL;
    obj->z_content[0] = '\0';
    obj->z_obj_sz = sizeof(zTCell);
    obj->z_line_flg = 1;
    
    /* Set child pointer of base object */
    obj->z_parent.z_draw_func = _ztcell_draw;

    /* Set child pointer of parent object and
     * return parent object */
    obj->z_parent.z_child = (void*) obj;
    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zTCell_Delete(zTCell* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* If object was internally created,
     * destroy it */
    if(obj->z_int_flg)
	free(obj);
}

/* Draw function */
int zTCell_Draw(zTCell* obj)
{
    #define ZTCELL_TEXT_LEFT 2.4
    #define ZTCELL_TEXT_TOP 1.5
    cairo_t* _dev_c;
    zBase* _base;
    zGeneric* _generic;
    PangoLayout* _layout;
    PangoFontDescription* _desc;
    double _x, _y;
    
    /* check object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    _base = &obj->z_parent;
    _generic = &obj->z_parent.z_sgeneric;

    /* Check object */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_generic);

    /* Get cairo device context */
    _dev_c = zGeneric_Get_Dev_Context(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* Check width and height */
    if(_base->z_width <= 0.0 &&
       _base->z_height <= 0.0)
	return 1;

    /* Save context */
    cairo_save(_dev_c);

    /* Draw rectangle */
    if(obj->z_line_flg)
	{
	    cairo_save(_dev_c);
	    cairo_rectangle(_dev_c,
	    		    CONV_TO_POINTS(_base->z_x),
	    		    CONV_TO_POINTS(_base->z_y),
	    		    CONV_TO_POINTS(_base->z_width),
	    		    CONV_TO_POINTS(_base->z_height));
	    cairo_stroke(_dev_c);
	    cairo_restore(_dev_c);
	}

    if(obj->z_content[0] != '\0')
    	{
    	    /* Translate to coordinates */
    	    _x = _base->z_x + ZTCELL_TEXT_LEFT;
    	    _y = _base->z_y + ZTCELL_TEXT_TOP;
    
    	    cairo_translate(_dev_c, CONV_TO_POINTS(_x), CONV_TO_POINTS(_y));

    	    /* Create pango layout */
    	    _layout = pango_cairo_create_layout(_dev_c);
    
    	    /* Add text to layout */
    	    pango_layout_set_text(_layout, obj->z_content, -1);
    
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
    _generic = NULL;
    _layout = NULL;
    _desc = NULL;

    return 0;
}

/* Private functions */
/*******************************************************************************************************/

/* Virtual draw function */
static int _ztcell_draw(zGeneric* obj)
{
    Z_CHECK_OBJ(obj);
    return zTCell_Draw(Z_TCELL(obj));
}
