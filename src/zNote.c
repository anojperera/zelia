/* Implementation of zNote class */
/* Sat Mar 10 13:25:25 GMT 2012 */

#include "zNote.h"
#include "zVar.h"
#include <stdio.h>
#include <string.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

/* Virtual function */
static int _znote_draw(zGeneric* obj);

/* Constructor */
zGeneric* zNote_New(zNote* obj)
{
    /* Call constructor helper */
    Z_CONSTRUCTOR_HELPER(obj, zNote);

    /* Create parent object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->z_ix = 0;			/* index set to nothing */
    obj->z_note[0] = '\0';
    obj->z_fnote = NULL;
    obj->z_indent = Z_NOTE_INDENT;	/* default note index */
    obj->z_note_sz = 0;
    obj->z_obj_sz = sizeof(zNote);
    obj->z_draw_func = NULL;
    obj->z_height_func = NULL;
    obj->z_usr_data = NULL;
    obj->z_child = NULL;

    /* Set child and function pointers of parent object */
    obj->z_parent.z_draw_func = _znote_draw;
    obj->z_parent.z_child = (void*) obj;

    /* Return parent object */
    return &obj->z_parent.z_sgeneric;
}


/* Destructor */
void zNote_Delete(zNote* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call destructor of parent object */
    zBase_Delete(&obj->z_parent);

    obj->z_draw_func = NULL;
    obj->z_child = NULL;

    /* If object was internally created free it */
    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zNote_Draw(zNote* obj)
{
    zGeneric* _generic;				/* generic object */
    cairo_t* _dev_c;				/* device context */
    char* _ix_buff;				/* index buffer */
    PangoLayout* _layout;			/* pango layout */
    PangoFontDescription* _desc;    		/* pango font description */
    double _box_width;

    /* check for object */
    Z_CHECK_OBJ(obj);
    
    _generic = &obj->z_parent.z_sgeneric;
    Z_CHECK_OBJ(_generic);

    /* Get device context */
    _dev_c = zGeneric_Get_Dev_Context(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* check if content was assigned */
    if(obj->z_note[0] == '\0' || obj->z_ix <= 0)
	return 1;

    /* get index into a buffer */
    _ix_buff = (char*) malloc(sizeof(int)+sizeof(char));
    sprintf(_ix_buff, "%i.", obj->z_ix);
    
    /* save cairo context */
    cairo_save(_dev_c);

    /* translate to coordinates */
    cairo_translate(_dev_c,
		    CONV_TO_POINTS(obj->z_parent.z_x),
		    CONV_TO_POINTS(obj->z_parent.z_y));

    _layout = pango_cairo_create_layout(_dev_c);
    pango_layout_set_width(_layout, CONV_TO_PANGO(obj->z_indent));

    /* add text to layout */
    pango_layout_set_text(_layout, _ix_buff, -1);

    /* create font description and add to layout */
    _desc = pango_font_description_from_string(Z_NOTE_FONT_STYLE);
    pango_layout_set_font_description(_layout, _desc);
    pango_font_description_free(_desc);

    /* show layout */
    pango_cairo_show_layout(_dev_c, _layout);

    /* offset cairo context for the text */
    cairo_translate(_dev_c,
		    CONV_TO_POINTS(obj->z_indent),
		    0.0);
    /* update pango layout */
    pango_cairo_update_layout(_dev_c, _layout);

    /* if width was set, tell pango to wrap */
    _box_width = obj->z_parent.z_width - obj->z_indent;
    if(obj->z_parent.z_width > 0.0)
	pango_layout_set_width(_layout, CONV_TO_PANGO(_box_width));
    
    /* update text */
    pango_layout_set_text(_layout, obj->z_note, -1);

    /* show layout */
    pango_cairo_show_layout(_dev_c, _layout);

    /* If height inform function was set call it */
    if(obj->z_height_func)
	obj->z_height_func(_generic, obj->z_usr_data, pango_layout_get_height(_layout));

    /* free layout object */
    g_object_unref(_layout);
    free(_ix_buff);
    
    /* restore cairo context */
    cairo_restore(_dev_c);

    _generic = NULL;
    _dev_c = NULL;
    _ix_buff = NULL;
    _layout = NULL;
    _desc = NULL;
    return 0;
}

/***************************************************************************/
/* Property methods */

/* Set content */
inline int zNote_Set_Content(zNote* obj, const char* content, int ix)
{
    /* check object and argument */
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(content);

    obj->z_ix = ix;
    obj->z_note_sz = strlen(content);
    if(obj->z_fnote)
	{
	    free(obj->z_fnote);
	    obj->z_fnote = NULL;
	}
    /* if length exceeds buffer size, return fail */
    if(obj->z_note_sz < Z_NOTE_BUFF)
	strcpy(obj->z_note, content);
    else
	return 1;
    
    return 0;
}

/* Get content */
inline const char* zNote_Get_Content(zNote* obj)
{
    /* check object */
    Z_CHECK_OBJ_PTR(obj);

    if(obj->z_note[0] != '\0')
	return obj->z_note;
    else
	return NULL;
}

/* Get index */
inline int zNote_Get_Ix(zNote* obj)
{
    Z_CHECK_OBJ(obj);
    return obj->z_ix;
}

/* Get note and index */
inline const char* zNote_Get_Note_With_Ix(zNote* obj)
{
    size_t _t;
    Z_CHECK_OBJ_PTR(obj);
    if(obj->z_note[0] == '\0' || obj->z_ix <= 0)
	return NULL;

    /* If the fnote was created in a previous call
     * return it */
    if(obj->z_fnote)
	return obj->z_fnote;

    _t = sizeof(int) + sizeof(char) * 2 + Z_NOTE_BUFF;
    obj->z_fnote = (char*) malloc(_t);
    /* copy index to buffer */
    sprintf(obj->z_fnote, "%i. ", obj->z_ix);
    strcat(obj->z_fnote, obj->z_note);

    return obj->z_fnote;
}

/* Set indent */
inline int zNote_Set_Indent(zNote* obj, double indent)
{
    Z_CHECK_OBJ(obj);
    obj->z_indent = indent;
    return 0;
}

/* Get indent */
inline double zNote_Get_Indent(zNote* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_indent;
}

/* Virtual method */
/* Virtual draw method */
static int _znote_draw(zGeneric* obj)
{
    Z_CHECK_OBJ(obj);
    return zNote_Draw(Z_NOTE(obj));
}
