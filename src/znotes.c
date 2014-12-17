/* Implementaion of zNotes collection */
/* Sat Mar 10 20:10:03 GMT 2012 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>
#include <blist.h>

#include "zVar.h"
#include "znotes.h"


static int _znotes_draw_helper(znotes* obj);

/* Virtual functions */
static int _znotes_draw(void* obj);
static int _znotes_delete(void* obj);
static int _znotes_get_note_height(zgeneric* obj, void* usr_data, int height);

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
    if(!(obj->super_cls = zgenerics_new(&obj->parent, 1, 0)))
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
    obj->title = NULL;
    obj->uline_flg = 1;
    obj->counter = 0;
    obj->_znotes_counter = 0;

    obj->child = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    /* Set function poiters of parent object */
    zgeneric_set_draw(obj, _znotes_draw);
    zgeneric_set_delete_callback(obj, _znotes_delete);

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
	{
	    obj->vtable.zgeneric_delete((void*) obj->super_cls);
	    return;
	}

    /* Call delete method of parent object */
    zgeneric_block_parent_destructor(obj);
    zgenerics_delete(&obj->parent);

    ZGENERIC_INIT_VTABLE(obj);
    if(obj->title)
	free(obj->title);
    obj->title = NULL;
    obj->super_cls = NULL;
    obj->child = NULL;

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
    ZCHECK_OBJ_INT(obj);
    ZCHECK_OBJ_INT(note);

    /* Check if title of the notes were set if
     * not return it */
    if(obj->title == NULL)
	return ZELIA_NOTES_ERROR;

    _zg = znote_new(NULL);
    ZCHECK_OBJ_INT(_zg);

    _note = Z_NOTE(_zg);

    zgeneric_set_device(_zg, zgenerics_get_device(&obj->parent));
    zgeneric_set_default_dev_context(_zg);

    /* increment counter and set note content */
    if(zgenerics_get_ref_flg(obj->super_cls) > 0)
	znote_set_content_with_ref(_note, note, ++obj->counter);
    else
	znote_set_content(_note, note, ++obj->counter);

    obj->_height = (double) (obj->counter + 1) * Z_NOTE_LINE_HEIGHT;

    /* Set base coordinates */
    zbase_set_base_coords(Z_BASE(_zg),
			  obj->x,
			  obj->y + (double) obj->counter * Z_NOTE_LINE_HEIGHT);
    zbase_set_width(Z_BASE(_zg), obj->width);

    /* Set inform function pointer */
    _note->usr_data = (void*) obj;
    _note->height_func = _znotes_get_note_height;

    /* set collection pointer */
    zgeneric_set_collection_pointer(_zg, obj);

    /* increment counter of parent */
    zgenerics_increment_counter(Z_GENERICS(obj));

    /* add to collection */
    blist_add_from_end(&obj->parent.generics_d, _zg);

    return 0;
}

/* Get Note */
znote* znotes_get_note(znotes* obj, unsigned int ix)
{
    void* _obj = NULL;
    zgeneric* _zg = NULL;
    unsigned int _count = 0;
    struct _blist_elm* _head = NULL;
    /* check object */
    ZCHECK_OBJ_PTR(obj);

    /* check array bounds */
    if(ix >= obj->counter)
	return NULL;

    /* Get node */
    _head = blist_get_head(&obj->super_cls->generics_d);
    while(_head)
	{
	    if(_count++ == ix)
		break;

	    _head = blist_next(_head);
	}

    if(_head == NULL || (_obj = blist_data(_head)) == NULL)
	return NULL;

    _zg = (zgeneric*) _obj;

    return Z_NOTE(_zg);
}


/*=================================== Private Methods ===================================*/

/* Virtual draw function */
static int _znotes_draw(void* obj)
{
    znotes* _zns;
    znote* _zn;
    znote* _zn_prev;			/* previous note */

    /* check for object */
    ZCHECK_OBJ_INT(obj);
    _zn = (znote*) obj;
    _zns = (znotes*) zgeneric_get_collection_pointer(_zn);

    /* Reset counter */
    if(_zns->_znotes_counter > _zns->parent.count)
	_zns->_znotes_counter = 0;

    if(_zns->_znotes_counter == 0)
	_znotes_draw_helper(_zns);

    /* Get previous object and set height if required */
    _zn_prev = znotes_get_note(_zns, _zns->_znotes_counter);
    if(_zn_prev)
	{
	    if((_zns->_note_height +_zn_prev->parent.y - _zn->parent.y) > Z_NOTE_LINE_HEIGHT)
		_zn->parent.y = _zn_prev->parent.y + _zns->_note_height * 2;
	}

    _zns->_znotes_counter++;
    return znote_draw(_zn);
}

/* Virtual delete function */
static int _znotes_delete(void* obj)
{
    zgenerics* _zgs = NULL;

    /* check object */
    ZCHECK_OBJ_INT(obj);
    _zgs = (zgenerics*) obj;

    znotes_delete(Z_NOTES(_zgs));
    return ZELIA_OK;
}

/* Draw helper function draws the title */
static int _znotes_draw_helper(znotes* obj)
{
    zdevice* _device;
    cairo_t* _dev_c;
    PangoLayout* _layout;
    PangoFontDescription* _desc;
    PangoAttrList* _attr_list;
    PangoAttribute* _attr;

    ZCHECK_OBJ_INT(obj);

    /* check if title was set */
    if(obj->title == NULL)
	return ZELIA_NOTES_ERROR;

    /* Get device object */
    _device = zgenerics_get_device(obj->super_cls);
    ZCHECK_OBJ_INT(_device);

    _dev_c = zdevice_get_context(_device);
    ZCHECK_OBJ_INT(_dev_c);

    /* save cairo context */
    cairo_save(_dev_c);

    /* translate cairo context */
    cairo_translate(_dev_c,
		    ZCONV_TO_POINTS(obj->x),
		    ZCONV_TO_POINTS(obj->y));

    /* Create layout object */
    _layout = pango_cairo_create_layout(_dev_c);
    if(obj->width > 0)
	pango_layout_set_width(_layout, ZCONV_TO_PANGO(obj->width));

    /* set text */
    pango_layout_set_text(_layout, obj->title, -1);

    /* create font description */
    _desc = pango_font_description_from_string(Z_GRD_FONT_STYLE);
    pango_layout_set_font_description(_layout, _desc);
    pango_font_description_free(_desc);

    /* Create attribute list */
    if(obj->uline_flg)
	{
	    _attr_list = pango_attr_list_new();
	    _attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
	    pango_attr_list_insert(_attr_list, _attr);

	    /* set list to layout */
	    pango_layout_set_attributes(_layout, _attr_list);
	}
    pango_cairo_show_layout(_dev_c, _layout);

    /* pango_attribute_destroy(_attr); */
    if(obj->uline_flg)
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
static int _znotes_get_note_height(zgeneric* obj, void* usr_data, int height)
{
    znotes* _notes;

    /* Check objects */
    ZCHECK_OBJ_INT(obj);
    ZCHECK_OBJ_INT(usr_data);

    _notes = (znotes*) usr_data;
    if(height <= 0)
	{
	    _notes->_note_height = Z_NOTE_LINE_HEIGHT * fabs((double) height);;
	    return 1;
	}

    _notes->_note_height = pango_units_to_double(height);
    return 0;
}
