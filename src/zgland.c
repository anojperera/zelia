/* Implementation of zGland */
/* Fri Feb  3 14:35:35 GMTST 2012 */

#include <math.h>
#include "zVar.h"
#include "zgland.h"

#define ZGLAND_COORDS 6

/* Virtual functions */
static int _zgland_draw(void* obj);
static int _zgland_delete(void* obj);

/* Constructor */
zgeneric* zgland_new(zgland* obj,		/* optional object */
		     zdevice* dev,		/* optional device object */
		     double x,			/* x coordinate */
		     double y, 			/* y coordinate */
		     zGlandSize sz)
{

    ZCONSTRUCTOR(obj, zgland);

    /* Create base object */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set device */
    if(dev)
	{
	    zgeneric_set_device(obj->super_cls, dev);
	    zgeneric_set_default_dev_context(obj->super_cls);
	}

    /* set properties */
    obj->gland_size = sz;
    zbase_set_base_coords(&obj->parent, x, y);
    switch(sz)
	{
	case zM16:
	    obj->dia = Z_M16_GLAND;
	    break;
	case zM20:
	    obj->dia = Z_M20_GLAND;
	    break;
	case zM25:
	    obj->dia = Z_M25_GLAND;
	    break;
	default:
	    obj->dia = Z_M20_GLAND;
	}

    obj->child = NULL;
    obj->hex_flg = 0;

    /* Set function pointers of parent object */
    zgeneric_set_draw(obj, _zgland_draw);
    zgeneric_set_delete_callback(obj, _zgland_delete);

    /* Set child pointer of parent object and return
     * top level object */
    zgeneric_set_child_pointer(obj);

    return obj->super_cls;
}


/* Destructor */
void zgland_delete(zgland* obj)
{
    ZCHECK_OBJ_VOID(obj);


    /* Call destructor of parent object */
    zgeneric_block_parent_destructor(obj);
    zbase_delete(&obj->parent);


    obj->child = NULL;
    obj->super_cls = NULL;

    if(ZDESTRUCTOR_CHECK)
	free(obj);
    return;
}

/* Draw method */
int zgland_draw(zgland* obj)
{

    zbase* _base;			/* base object pointer*/
    cairo_t* _dev_c;			/* cairo device context */
    double _rad;			/* radius */
    int _i;
    double _x1[ZGLAND_COORDS], _y1[ZGLAND_COORDS], _x2[ZGLAND_COORDS], _y2[ZGLAND_COORDS];

    /* Check object */
    ZCHECK_OBJ_INT(obj);

    _base = &obj->parent;

    /* Check object pointers */
    ZCHECK_OBJ_INT(_base);

    /* Get cairo device context */
    _dev_c = zgeneric_get_dev_context(obj->super_cls);
    ZCHECK_OBJ_INT(_dev_c);

    /* draw outer circles */
    _rad = obj->dia / 2;
    cairo_arc(_dev_c,
	      ZCONV_TO_POINTS(_base->x),
	      ZCONV_TO_POINTS(_base->y),
	      ZCONV_TO_POINTS(_rad),
	      0.0,
	      2 * M_PI);

    /* draw hex */
    _x1[0] = _base->x - _rad * sin(M_PI /6);
    _y1[0] = _base->y - _rad * cos(M_PI / 6);
    _x2[0] = _base->x + _rad * sin(M_PI /6);
    _y2[0] = _y1[0];

    _x1[1] = _x2[0];
    _y1[1] = _y2[0];
    _x2[1] = _base->x + _rad;
    _y2[1] = _base->y;

    _x1[2] = _x2[1];
    _y1[2] = _y2[1];
    _x2[2] = _x2[0];
    _y2[2] = _base->y + _rad * cos(M_PI /6);

    _x1[3] = _x2[2];
    _y1[3] = _y2[2];
    _x2[3] = _x1[0];
    _y2[3] = _y1[3];

    _x1[4] = _x2[3];
    _y1[4] = _y2[3];
    _x2[4] = _base->x - _rad;
    _y2[4] = _base->y;

    _x1[5] = _x2[4];
    _y1[5] = _y2[4];
    _x2[5] = _x1[0];
    _y2[5] = _y1[0];

    if(obj->hex_flg)
	{
	    for(_i=0; _i<ZGLAND_COORDS; _i++)
		{
		    cairo_move_to(_dev_c, ZCONV_TO_POINTS(_x1[_i]), ZCONV_TO_POINTS(_y1[_i]));
		    cairo_line_to(_dev_c, ZCONV_TO_POINTS(_x2[_i]), ZCONV_TO_POINTS(_y2[_i]));
		}
	}
    cairo_stroke(_dev_c);

    return 0;
}


/*=================================== Private Methods ===================================*/
static int _zgland_draw(void* obj)
{
    zgeneric* _zg = NULL;
    zgland* _self = NULL;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;
    _self = Z_GLAND(_zg);

    return zgland_draw(_self);

}

static int _zgland_delete(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    zgland_delete(Z_GLAND(_zg));

    return ZELIA_OK;
}
