/* Implementation of Cell class of table */
/* Sat Mar  3 13:16:04 GMT 2012 */

#include "zTCell.h"

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
    obj->_z_cell_width = 0.0;
    obj->_z_cell_height = 0.0;
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
    cairo_t* _dev_c;
    zBase* _base;
    zGeneric* _genric;
    PangoLayout* _layout;
    PangoFontDescription* _desc;
    double _x, _y;
    
    /* check object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    _base = &obj->z_parent;
    _genric = &obj->z_parent.z_sgeneric;

    /* Check object */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_genric);

    /* Get cairo device context */
    _dev_c = zGeneric_Get_Dev_Context(_genric);
    Z_CHECK_OBJ(_dev_c);

    /* Check width and height */
    if(obj->_z_cell_width <= 0.0 &&
       obj->_z_cell_height <= 0.0)
	return 1;

    /* Save context */
    cario_save(_dev_c);

    /* Draw rectangle */
    if(obj->z_line_flg)
	{
	    cairo_save(_dev_c);
	    cairo_rectangle(_dev_c,
			    CONV_TO_POINTS(_base->z_x),
			    CONV_TO_POINTS(_base->z_y),
			    CONV_TO_POINTS(obj->_z_cell_width),
			    CONV_TO_POINTS(obj->_z_cell_height));
	    cairo_stroke(_dev_c);
	    cairo_restore(_dev_c);
	}

    /* Translate to coordinates */
    _x = _base->z_x + obj->_z_cell_width * 0.8;
    _y = _base->z_y + obj->_z_cell_height * 0.6;
    
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

    /* Restore cairo context */
    cairo_restore(_dev_c);

    /* Free layout object */
    g_object_unref(_layout);
    
    _dev_c = NULL;
    _base = NULL;
    _genric = NULL;
    _layout = NULL;
    _desc = NULL;

    return 0;
}

/* Set cell content */
inline int zTCell_Set_Content(zTCell* obj, const char* content)
{
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(content);
    strcpy(obj->z_content, content);
    return 0;
}

/* Get cell content */
inline const char* zTCell_Get_Content(zTCell* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_content;
}

/* Get Row index */
inline unsigned int zTCell_Get_Row_Ix(zTCell* obj)
{
    Z_CHECK_OBJ(obj);
    return obj->_z_row_ix;
}

/* Get Column index */
inline unsigned int zTCell_Get_Col_Ix(zTCell* obj)
{
    Z_CHECK_OBJ(obj);
    return obj->_z_col_ix;
}

/* Get cell width */
inline double zTCell_Get_Width(zTCell* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->_z_cell_width;
}

/* Get cell height */
inline double zTCell_Get_Height(zTCell* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->_z_cell_height;
}

/* Set line flag */
inline int zTCell_Set_Line_Flg(zTCell* obj, unsigned int flg)
{
    Z_CHECK_OBJ(obj);
    obj->z_line_flg = flg;
    return 0;
}

/* Get line flag */
inline unsigned int zTCell_Get_Line_Flg(zTCell* obj)
{
    if(obj == NULL)
	return 0;
    else
	return obj->z_line_flg;
}
