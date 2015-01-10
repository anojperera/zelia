/* Implementation of Arrow head */
/* Mon Feb  6 22:16:39 GMT 2012 */

#include <math.h>
#include "zarrow.h"

#define ZARROW_ARR 6

/* Virtual draw function */
static int _zarrow_draw(void* obj);
static int _zarrow_delete(void* obj);

/* Constructor */
zgeneric* zarrow_new(zarrow* obj)
{
    /* check for object */
    ZCONSTRUCTOR(obj, zarrow);

    /* Create base object */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set variables */
    obj->arrow_type = zArrow1;
    obj->fill_flg = 1;				/* fill arrow head */
    obj->child = NULL;


    /* Assign function pointer of parent object */
    zgeneric_set_draw(obj, _zarrow_draw);
    zgeneric_set_delete_callback(obj, _zarrow_delete);

    /* Set child pointer of parent object */
    zgeneric_set_child_pointer(obj);

    /* set child pointer of base object */
    return obj->super_cls;
}


/* Destructor */
void zarrow_delete(zarrow* obj)
{
    ZCHECK_OBJ_VOID(obj);

    /* call destructor of base object */
    zgeneric_block_parent_destructor(obj);
    zbase_delete(&obj->parent);

    obj->child = NULL;
    obj->super_cls = NULL;

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Draw function */
int zarrow_draw(zarrow* obj)
{

    zbase* _base;
    cairo_t* _dev_c;
    double x[ZARROW_ARR], y[ZARROW_ARR], h, ang;
    int i;

    /* check for object */
    ZCHECK_OBJ_INT(obj);

    /* Get base and generic object */
    _base = &obj->parent;

    /* Device context pointer */
    _dev_c = zgeneric_get_dev_context(obj->super_cls);

    /* Check all objects */
    ZCHECK_OBJ_INT(_base);
    ZCHECK_OBJ_INT(_dev_c);

    ZELIA_LOG_MESSAGE("zarrow start drawing");

    /* Check if the angle is greater than zero.
     * If the angle is greater than zero, save cairo context
     * and translate and rotate the coordinate system */
    cairo_save(_dev_c);

    cairo_translate(_dev_c,
		    ZCONV_TO_POINTS(_base->x),
		    ZCONV_TO_POINTS(_base->y));

    if(_base->ang > 0)
	{
	    cairo_rotate(_dev_c,
			 ZCONV_TO_RADIANS(_base->ang));
	}

    switch(obj->arrow_type)
	{
	case zArrow2:
	    ang = Z_ARROW_ANG2;
	    h = Z_ARROW_HEIGHT2;
	    break;
	case zArrow3:
	    ang = Z_ARROW_ANG2;
	    h = Z_ARROW_HEIGHT3;
	    break;
	default:
	    ang = Z_ARROW_ANG1;
	    h = Z_ARROW_HEIGHT1;
	}

    x[0] = 0.0;
    y[0] = 0.0;
    x[1] = -1 * h * tan(ZCONV_TO_RADIANS(ang));
    y[1] = -1 * h;

    x[2] = x[1];
    y[2] = y[1];
    x[3] = h * tan(ZCONV_TO_RADIANS(ang));
    y[3] = y[2];

    x[4] = x[3];
    y[4] = y[3];
    x[5] = 0;
    y[5] = 0;

    cairo_move_to(_dev_c,
		  ZCONV_TO_POINTS(x[0]),
		  ZCONV_TO_POINTS(y[0]));
    for(i = 1; i < ZARROW_ARR; i++)
	{
	    cairo_line_to(_dev_c,
			  ZCONV_TO_POINTS(x[i]),
			  ZCONV_TO_POINTS(y[i]));
	}

    if(obj->fill_flg)
	cairo_fill(_dev_c);
    else
	cairo_stroke(_dev_c);

    if(_base->ang > 0)
	cairo_rotate(_dev_c, 0.0);

    cairo_translate(_dev_c,
		    -1 * ZCONV_TO_POINTS(_base->x),
		    -1 * ZCONV_TO_POINTS(_base->y));

    cairo_restore(_dev_c);

    ZELIA_LOG_MESSAGE("zarrow compelete drawing");    
    return ZELIA_OK;
}


/*========================================================================*/
/* Virtual function implementation */

/* Draw method */
static int _zarrow_draw(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    return zarrow_draw(Z_ARROW(_zg));
}

static int _zarrow_delete(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    zarrow_delete(Z_ARROW(_zg));
    return  ZELIA_OK;
}
