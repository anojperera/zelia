/* Implementation of zNote class */
/* Sat Mar 10 13:25:25 GMT 2012 */

#include "zVar.h"
#include <stdio.h>
#include <string.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include "znote.h"

/* Virtual function */
static int _znote_draw(void* obj);
static int _znote_delete(void* obj);

/* Constructor */
zgeneric* znote_new(znote* obj)
{
    
    ZCONSTRUCTOR(obj, znote);

    /* Create parent object */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->ix = 0;				/* index set to nothing */
    obj->note = NULL;
    obj->fnote = NULL;
    obj->indent = Z_NOTE_INDENT;		/* default note index */
    obj->note_sz = 0;

    /*
     * flag to indicate content is referenced,
     * this prevents the object copying the string to internal memory
     */
    obj->ref_flg = 0;


    obj->height_func = NULL;
    obj->usr_data = NULL;
    obj->cols = NULL;
    obj->child = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    /* Set child and function pointers of parent object */
    zgeneric_set_draw(obj, _znote_draw);
    zgeneric_set_delete_callback(obj, _znote_delete);

    zgeneric_set_child_pointer(obj);

    /* Return parent object */
    return obj->super_cls;
}


/* Destructor */
void znote_delete(znote* obj)
{
    ZCHECK_OBJ_VOID(obj);

    if(obj->vtable.zgeneric_delete)
	{
	    obj->vtable.zgeneric_delete((void*) obj->super_cls);
	    return;
	}

    /* Call destructor of parent object */
    zgeneric_block_parent_destructor(obj);
    zbase_delete(&obj->parent);


    if(obj->note)
	free(obj->note);
    if(obj->fnote)
	free(obj->note);
    obj->note = NULL;
    obj->fnote = NULL;
    obj->super_cls = NULL;
    obj->child = NULL;
    obj->cols = NULL;
    obj->usr_data = NULL;
    obj->height_func = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    /* If object was internally created free it */
    if(ZDESTRUCTOR_CHECK)
	free(obj);
    
    return;
}

/* Draw method */
int znote_draw(znote* obj)
{
    cairo_t* _dev_c;				/* device context */
    char* _ix_buff;				/* index buffer */
    PangoLayout* _layout;			/* pango layout */
    PangoFontDescription* _desc;    		/* pango font description */
    double _box_width;

    /* check for object */
    ZCHECK_OBJ_INT(obj);
    

    /* Get device context */
    _dev_c = zgeneric_get_dev_context(obj->super_cls);
    ZCHECK_OBJ_INT(_dev_c);

    /* check if content was assigned */
    if(obj->note == NULL || obj->ix <= 0)
	return ZELIA_NOTE_ERROR;

    /* get index into a buffer */
    _ix_buff = (char*) malloc(sizeof(int)+sizeof(char));
    sprintf(_ix_buff, "%i.", obj->ix);
    
    /* save cairo context */
    cairo_save(_dev_c);

    /* translate to coordinates */
    cairo_translate(_dev_c,
		    ZCONV_TO_POINTS(obj->parent.x),
		    ZCONV_TO_POINTS(obj->parent.y));

    _layout = pango_cairo_create_layout(_dev_c);
    pango_layout_set_width(_layout, ZCONV_TO_PANGO(obj->indent));

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
		    ZCONV_TO_POINTS(obj->indent),
		    0.0);
    /* update pango layout */
    pango_cairo_update_layout(_dev_c, _layout);

    /* if width was set, tell pango to wrap */
    _box_width = obj->parent.width - obj->indent;
    if(obj->parent.width > 0.0)
	pango_layout_set_width(_layout, ZCONV_TO_PANGO(_box_width));
    
    /* update text */
    pango_layout_set_text(_layout, obj->note, -1);

    /* show layout */
    pango_cairo_show_layout(_dev_c, _layout);

    /* If height inform function was set call it */
    if(obj->height_func)
	obj->height_func(obj->super_cls, obj->usr_data, pango_layout_get_height(_layout));

    /* free layout object */
    g_object_unref(_layout);
    free(_ix_buff);
    
    /* restore cairo context */
    cairo_restore(_dev_c);

    _dev_c = NULL;
    _ix_buff = NULL;
    _layout = NULL;
    _desc = NULL;
    
    return ZELIA_OK;
}

/*=================================== Property Methods ===================================*/

/* Set content */
int znote_set_content(znote* obj, const char* content, int ix)
{
    /* check object and argument */
    ZCHECK_OBJ_INT(obj);
    ZCHECK_OBJ_INT(content);

    obj->ix = ix;
    obj->note_sz = strlen(content);
    if(obj->fnote)
	{
	    free(obj->fnote);
	    obj->fnote = NULL;
	}
    
    if(obj->note)
	free(obj->note);

    if(obj->ref_flg == 0)
	{
	    obj->note = (char*) malloc(sizeof(char) * (obj->note_sz + 1));
	    if(!strcpy(obj->note, content))
		return ZELIA_NOTE_ERROR;
	}
    else
	obj->note = (char*) content;
        
    return ZELIA_OK;
}

int znote_set_content_with_ref(znote* obj, const char* content, int ix)
{
    /* check arguments */
    ZCHECK_OBJ_INT(obj);
    ZCHECK_OBJ_INT(content);

    /* set the reference flag */
    obj->ref_flg = 1;
    return znote_set_content(obj, content, ix);
}


/* Get note and index */
const char* znote_get_note_with_ix(znote* obj)
{
    size_t _t;
    
    ZCHECK_OBJ_PTR(obj);
    if(obj->note == NULL || obj->ix <= 0)
	return NULL;

    /* If the fnote was created in a previous call
     * return it */
    if(obj->fnote)
	return obj->fnote;

    /* this section should be more optimised */
    _t = sizeof(int) + sizeof(char) * 2 + Z_NOTE_BUFF;
    obj->fnote = (char*) malloc(_t);
    
    /* copy index to buffer */
    sprintf(obj->fnote, "%i. ", obj->ix);
    strcat(obj->fnote, obj->note);

    return obj->fnote;
}


/*=================================== Private Methods ===================================*/
/* Virtual draw method */
static int _znote_draw(void* obj)
{
    zgeneric* _zg = NULL;
    
    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;
    
    return znote_draw(Z_NOTE(_zg));
}

/* Delete callback from parent */
static int _znote_delete(void* obj)
{
    zgeneric* _zg = NULL;
    
    ZCHECK_OBJ_INT(obj);
    
    _zg = (zgeneric*) obj;
        
    znote_delete(Z_NOTE(_zg));
    return ZELIA_OK;
}
