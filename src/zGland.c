/* Implementation of zGland */
/* Fri Feb  3 14:35:35 GMTST 2012 */

#include <math.h>
#include "zGland.h"

/* Virtual functions */
static int _zgland_draw(zGeneric* obj);

/* Constructor */
zGeneric* zGland_New(zGland* obj,		/* optional object */
		     zDevice* dev,		/* optional device object */
		     double x,			/* x coordinate */
		     double y, 			/* y coordinate */
		     zGlandSize sz)
{
    /* Check object */
    if(obj == NULL)
	{
	    obj = (zGland*) malloc(sizeof(zGland));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create base object */
    if(!zBase_New(&obj->z_parent))
	{
	    free(obj);
	    obj = NULL;
	    return NULL;
	}

    /* Set device */
    if(dev)
	{
	    zGeneric_Set_Device(&obj->z_parent.z_sgeneric, dev);
	    zGeneric_Set_Default_Dev_Context(&obj->z_parent.z_sgeneric);
	}

    /* set properties */
    obj->z_gland_size = sz;
    zBase_Set_Base_Coords(&obj->z_parent, x, y);
    switch(sz)
	{
	case zM16:
	    obj->z_dia = Z_M16_GLAND;
	    break;
	case zM20:
	    obj->z_dia = Z_M20_GLAND;
	    break;
	case zM25:
	    obj->z_dia = Z_M25_GLAND;
	    break;
	default:
	    obj->z_dia = Z_M20_GLAND;
	}

    obj->z_draw_func = NULL;
    obj->z_child = NULL;
    
    /* Set function pointers of parent object */
    obj->z_parent.z_draw_func = _zgland_draw;

    /* Set child pointer of parent object and return
     * top level object */
    obj->z_parent.z_child = (void*) obj;

    return &obj->z_parent.z_sgeneric;
}


/* Destructor */
void zGland_Delete(zGland* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* Call destructor of parent object */
    zBase_Delete(obj->z_parent);

    obj->z_child = NULL;

    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zGland_Draw(zGland* obj)
{
    #define COORDS 6
    zBase* _base;			/* base object pointer*/
    zGeneric* _genric;			/* generic object pointer */
    cairo_t* _dev_c;			/* cairo device context */
    double _rad;			/* radius */
    int i;
    double x1[COORDS], y1[COORDS], x2[COORDS], y2[COORDS];
    
    /* Check object */
    Z_CHECK_OBJ(obj);

    _base = &obj->z_parent;
    _genric = &obj->z_parent.z_sgeneric;

    /* Check object pointers */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_genric);
    
    /* Get cairo device context */
    _dev_c = zGeneric_Get_Dev_Context(_genric);
    Z_CHECK_OBJ(_dev_c);

    /* draw outer circles */
    _rad = obj->z_dia / 2;
    cairo_move_to(_dev_c, CONV_TO_POINTS(_base->z_x), CONV_TO_POINTS(_base->z_y));
    
    cairo_arc(_dev_c,
	      CONV_TO_POINTS(_base->z_x),
	      CONV_TO_POINTS(_base->z_y),
	      CONV_TO_POINTS(_rad),
	      0.0,
	      2 * M_PI);

    /* draw hex */
    x1[0] = _base->z_x - _rad * sin(M_PI /6);
    y1[0] = _base->z_y - _rad * cos(M_PI / 6);
    x2[0] = _base->z_x + _rad * sin(M_PI /6);
    y2[0] = y1[0];

    x1[1] = x2[0];
    y1[1] = y2[0];
    x2[1] = _base->z_x + _rad;
    y2[1] = _base->z_y;

    x1[2] = x2[1];
    y1[2] = y2[1];
    x2[2] = x2[0];
    y2[2] = _base->z_y + _rad * cos(M_PI /6);

    x1[3] = x2[2];
    y1[3] = y2[2];
    x2[3] = x1[0];
    y2[3] = y1[3];

    x1[4] = x2[3];
    y1[4] = y2[3];
    x2[4] = _base->z_x - _rad;
    y2[4] = _base->z_y;

    x1[5] = x2[4];
    y1[5] = y2[4];
    x2[5] = x1[0];
    y2[5] = y1[0];

    for(i = 0; i < COORDS; i++)
	{
	    cairo_move_to(_dev_c, CONV_TO_POINTS(x1[i]), CONV_TO_POINTS(y1[i]));
	    cairo_line_to(_dev_c, CONV_TO_POINTS(x2[i]), CONV_TO_POINTS(y2[i]));
	}

    cairo_stroke(_dev_c);
    
    return 0;
}
