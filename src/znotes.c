/* Implementaion of zNotes collection */
/* Sat Mar 10 20:10:03 GMT 2012 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>
#include <glist.h>

#include "zvar.h"
#include "znotes.h"


static int _znotes_counter = 0;
static int _znotes_draw_helper(znotes* obj);

/* Virtual functions */
static int _znotes_draw(void* obj, void* usr_data);
static int _znotes_delete(void* obj, void* usr_data);
static int _znotes_get_note_height(zGeneric* obj, void* usr_data, int height);

/* Constructor */
zgenerics* znotes_new(znotes* obj,		/* optional argument */
		      zdevice* dev,		/* device object */
		      double x,			/* base x coordinate */
		      double y,			/* base y coordinate */
		      double width)		/* width */
{
    /* constructor helper */
    ZCONSTRUCTOR(obj, znotes);

    /* Create parent object */
    if(!obj->super_cls = zgenerics_new(&obj->parent, 1, 0))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}
    zgenerics_set_device(&obj->parent, dev);
    
    /* Set properties */
    obj->_height = 0.0;
    obj->width = width;
    obj->x = x;
    obj->y = y;
    obj->_note_height = 0.0;
    obj->title[0] = '\0';
    obj->uline_flg = 1;
    obj->counter = 0;

    obj->child = NULL;
    ZGENERIC_INIT_VTABLE(obj);
    
    /* Set function poiters of parent object */
    zgeneric_set_delete_callback(obj, _znotes_delete);
    zgeneric_set_draw(obj, _znotes_draw);

    /* set child pointer of parent object */
    zgeneric_set_child_pointer(obj);
    
    obj->parent.usr_data = (void*) obj;

    /* return parent object */
    return obj->super_cls;
}

/* Destructor */
void znotes_delete(znotes* obj)
{
    /* Check object */
    ZCHECK_OBJ_VOID(obj);
    if(obj->vtable.zgeneric_delete)
	obj->vtable.zgeneric_delete((void*) obj->super_cls);

    /* Call delete method of parent object */
    zgenerics_delete(&obj->z_parent);

    ZGENERIC_INIT_VTABLE(obj);
    obj->super_cls = NULL;
    obj->z_child = NULL;

    /* Check if object internally created and
     * free */
    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Add Notes */
int znotes_add(znotes* obj, const char* note)
{
    znote* _note;
    zgeneric* _zg;
    
    /* Check object and arguments */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(note);

    /* Check if title of the notes were set if
     * not return it */
    if(obj->title[0] == '\0')
	return ZELIA_NOTES_ERROR;
    
    _zg = znote_new(NULL);
    ZCHECK_OBJ_INT(_zg);

    _note = Z_NOTE(_zg);
    
    /* Set the internal flag to true as the struct
     * shall be copied to internal memory of alist
     * and it needs freed */
    _note->int_flg = 1;

    zgeneric_set_device(_zg, zgenerics_get_device(&obj->parent));
    zgeneric_set_default_dev_context(_zg);

    /* increment counter and set note content */
    znote_set_content(_note, note, ++obj->counter);
    obj->_height = (double) (obj->counter + 1) * NOTE_LINE_HEIGHT;
    
    /* Set base coordinates */
    zbase_set_base_coords(Z_BASE(_zg),
			  obj->x,
			  obj->y + (double) obj->counter * NOTE_LINE_HEIGHT);
    zbase_set_width(Z_BASE(_zg), obj->width);

    /* Set inform function pointer */
    _note->usr_data = (void*) obj;
    _note->height_func = _znotes_get_note_height;

    /* set collection pointer */
    zgeneric_set_collection_pointer(_zg, obj);

    /* add to collection */
    blist_add_from_end(&obj->parent.generics_d, _zg);

    return 0;
}

/* Get Note */
znote* znotes_get_note(znotes* obj, unsigned int ix)
{
    void* _obj;
    zgeneric* _zg;
    unsigned int _count = 0;
    blist* _head;
    /* check object */
    ZCHECK_OBJ_PTR(obj);

    /* check array bounds */
    if(ix >= obj->counter)
	return NULL;

    /* Get node */
    _head = blist_get_head(&obj->super_cls->z_generics_d);
    while(_head)
	{
	    if(++_count == ix)
		break;

	    _head = blist_next(_head);
	}
    _obj = blist_data(_head);
    if(_obj == NULL)
	return NULL;

    _zg = (zgeneric*) _obj;
    
    return Z_NOTE(_zg);
}


/*=================================== Private Methods ===================================*/

/* Virtual draw function */
static int _znotes_draw(void* obj) 
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
    return znote_draw(_zn);
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
