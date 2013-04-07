/* Implementaion of zNotes collection */
/* Sat Mar 10 20:10:03 GMT 2012 */

#include "zNotes.h"
#include "zVar.h"
#include <stdio.h>
#include <string.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>
#include "../../g_list/src/alist.h"
#include <math.h>

static int _znotes_counter = 0;
static int _znotes_draw_helper(zNotes* obj);

/* Virtual functions */
static int _znotes_draw(void* obj, void* usr_data);
static int _znotes_delete(void* obj, void* usr_data);
static int _znotes_get_note_height(zGeneric* obj, void* usr_data, int height);

/* Constructor */
zGenerics* zNotes_New(zNotes* obj,		/* optional argument */
		      zDevice* dev,		/* device object */
		      double x,			/* base x coordinate */
		      double y,			/* base y coordinate */
		      double width)		/* width */
{
    /* constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zNotes);

    /* Create parent object */
    if(zGenerics_New(&obj->z_parent, 1, 0))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}
    zGenerics_Set_Device(&obj->z_parent, dev);
    
    /* Set properties */
    obj->_z_height = 0.0;
    obj->z_width = width;
    obj->z_x = x;
    obj->z_y = y;
    obj->_z_note_height = 0.0;
    obj->z_title[0] = '\0';
    obj->z_uline_flg = 1;
    obj->z_counter = 0;
    obj->z_obj_sz = sizeof(zNotes);
    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* Set function poiters of parent object */
    obj->z_parent.z_draw_func = _znotes_draw;
    obj->z_parent.z_destructor_func = _znotes_delete;

    /* set child pointer of parent object */
    obj->z_parent.z_child = (void*) obj;
    obj->z_parent.z_usr_data = (void*) obj;

    /* return parent object */
    return &obj->z_parent;
}

/* Destructor */
void zNotes_Delete(zNotes* obj)
{
    /* Check object */
    Z_CHECK_OBJ_VOID(obj);

    /* Call delete method of parent object */
    zGenerics_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* Check if object internally created and
     * free */
    if(obj->z_int_flg)
	free(obj);
}

/* Add Notes */
int zNotes_Add(zNotes* obj, const char* note)
{
    zNote _note;
    zGeneric* _zg;
    
    /* Check object and arguments */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(note);

    /* Check if title of the notes were set if
     * not return it */
    if(obj->z_title[0] == '\0')
	return 1;
    
    _zg = zNote_New(&_note);
    Z_CHECK_OBJ(_zg);
    
    /* Set the internal flag to true as the struct
     * shall be copied to internal memory of alist
     * and it needs freed */
    _note.z_int_flg = 1;

    zGeneric_Set_Device(_zg, zGenerics_Get_Device(&obj->z_parent));
    zGeneric_Set_Default_Dev_Context(_zg);

    /* increment counter and set note content */
    zNote_Set_Content(&_note, note, ++obj->z_counter);
    obj->_z_height = (double) (obj->z_counter + 1) * Z_NOTE_LINE_HEIGHT;
    
    /* Set base coordinates */
    zBase_Set_Base_Coords(Z_BASE(_zg),
			  obj->z_x,
			  obj->z_y + (double) obj->z_counter * Z_NOTE_LINE_HEIGHT);
    zBase_Set_Width(Z_BASE(_zg), obj->z_width);

    /* Set inform function pointer */
    _note.z_usr_data = (void*) obj;
    _note.z_height_func = _znotes_get_note_height;

    /* add to collection */
    aList_Add(&obj->z_parent.z_generics_d,
	      (void*) &_note,
	      sizeof(zNote));

    return 0;
}

/* Get Note */
inline zNote* zNotes_Get_Note(zNotes* obj, unsigned int ix)
{
    aNode* _node;
    
    /* check object */
    Z_CHECK_OBJ_PTR(obj);

    /* check array bounds */
    if(ix >= obj->z_counter)
	return NULL;

    /* Get node */
    _node = aList_Item(&obj->z_parent.z_generics_d, ix);
    if(_node == NULL)
	return NULL;
    return (zNote*) _node->data;
}

/* Get height */
inline double zNoets_Get_Height(zNotes* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->_z_height;
}

/* Set notes collection title */
inline int zNotes_Set_Title(zNotes* obj, const char* title)
{
    Z_CHECK_OBJ(obj);

    /* If the title string length exceeds buffer
     * return error */
    if(strlen(title) >= Z_NOTES_TITLE_BUFF)
	return 1;

    /* copy to internal buffer */
    strcpy(obj->z_title, title);
    return 0;
}

/* Get title */
inline const char* zNotes_Get_Title(zNotes* obj)
{
    /* check object */
    Z_CHECK_OBJ_PTR(obj);

    return obj->z_title[0] == '\0'? NULL : obj->z_title;
}

/* Set underline flag */
inline int zNotes_Set_Uline_Flg(zNotes* obj, unsigned int flg)
{
    Z_CHECK_OBJ(obj);
    obj->z_uline_flg = flg;
    return 0;
}

/**************************************************************************/
/* Private functions */

/* Virtual draw function */
static int _znotes_draw(void* obj, void* usr_data)
{
    zNotes* _zns;
    zNote* _zn;
    zNote* _zn_prev;			/* previous note */
    aNode* _node;
    
    /* check for object */
    Z_CHECK_OBJ(obj);
    _zn = (zNote*) obj;
    _zns = (zNotes*) usr_data;
    
    /* Reset counter */
    if(_znotes_counter > _zns->z_parent.z_count)
	_znotes_counter = 0;
    
    if(_znotes_counter == 0)
	_znotes_draw_helper(_zns);

    /* Get previous object and set height if required */
    _node = aList_Item(&_zns->z_parent.z_generics_d, _znotes_counter);
    if(_node)
	{
	    _zn_prev = (zNote*) _node->data;
	    if((_zns->_z_note_height +_zn_prev->z_parent.z_y - _zn->z_parent.z_y) > Z_NOTE_LINE_HEIGHT)
		_zn->z_parent.z_y = _zn_prev->z_parent.z_y + _zns->_z_note_height * 2;
	}
    _znotes_counter++;
    return zNote_Draw(_zn);
}

/* Virtual delete function */
static int _znotes_delete(void* obj, void* usr_data)
{
    zNote* _zn;
    if(obj)
	{
	    _zn = (zNote*) obj;
	    zNote_Delete(_zn);
	}
    return 0;
}

/* Draw helper function draws the title */
static int _znotes_draw_helper(zNotes* obj)
{
    zDevice* _device;
    cairo_t* _dev_c;
    PangoLayout* _layout;
    PangoFontDescription* _desc;
    PangoAttrList* _attr_list;
    PangoAttribute* _attr;
    
    Z_CHECK_OBJ(obj);

    /* check if title was set */
    if(obj->z_title[0] == '\0')
	return 1;

    /* Get device object */
    _device = zGenerics_Get_Device(&obj->z_parent);
    Z_CHECK_OBJ(_device);
    _dev_c = zDevice_Get_Context(_device);
    Z_CHECK_OBJ(_dev_c);
    
    /* save cairo context */
    cairo_save(_dev_c);

    /* translate cairo context */
    cairo_translate(_dev_c,
		    CONV_TO_POINTS(obj->z_x),
		    CONV_TO_POINTS(obj->z_y));

    /* Create layout object */
    _layout = pango_cairo_create_layout(_dev_c);
    if(obj->z_width > 0)
	pango_layout_set_width(_layout, CONV_TO_PANGO(obj->z_width));

    /* set text */
    pango_layout_set_text(_layout, obj->z_title, -1);

    /* create font description */
    _desc = pango_font_description_from_string(Z_GRD_FONT_STYLE);
    pango_layout_set_font_description(_layout, _desc);
    pango_font_description_free(_desc);

    /* Create attribute list */
    if(obj->z_uline_flg)
	{
	    _attr_list = pango_attr_list_new();
	    _attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
	    pango_attr_list_insert(_attr_list, _attr);

	    /* set list to layout */
	    pango_layout_set_attributes(_layout, _attr_list);
	}
    pango_cairo_show_layout(_dev_c, _layout);
    
    /* pango_attribute_destroy(_attr); */
    if(obj->z_uline_flg)
	pango_attr_list_unref(_attr_list);

    g_object_unref(_layout);
    
    /* restore cairo context */
    cairo_restore(_dev_c);

    _device = NULL;
    _dev_c = NULL;
    _layout = NULL;
    _desc = NULL;
    _attr_list = NULL;
    _attr = NULL;
    
    return 0;
}

/* Virtual inform height function */
static int _znotes_get_note_height(zGeneric* obj, void* usr_data, int height)
{
    zNotes* _notes;
    
    /* Check objects */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(usr_data);
    _notes = (zNotes*) usr_data;
    if(height <= 0)
	{
	    _notes->_z_note_height = Z_NOTE_LINE_HEIGHT * fabs((double) height);;
	    return 1;
	}

    _notes->_z_note_height = pango_units_to_double(height);
    return 0;	
}
