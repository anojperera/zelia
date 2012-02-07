/* Implementation of Arrow head */
/* Mon Feb  6 22:16:39 GMT 2012 */

#include "zArrow.h"
#include <math.h>

/* Virtual draw function */
static int _zarrow_draw_function(zGeneric* obj);


/* Constructor */
zGeneric* zArrow_New(zArrow* obj)
{
    /* check for object */
    if(obj == NULL)
	{
	    obj = (zArrow*) malloc(sizeof(zArrow));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create base object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set variables */
    obj->z_arrow_type = zArrow1;
    obj->z_fill_flg = 1;				/* fill arrow head */
    obj->z_ang = 0.0;					/* angle */
    obj->z_draw_func = NULL;
    obj->z_child = NULL;
    /* set draw function pointer of base object */
    obj->z_parent.z_draw_func = _zarrow_draw_function;

    /* set child pointer of base object */
    obj->z_parent.z_child = (void*) obj;
    return &obj->z_parent;
}


/* Destructor */
void zArrow_Delete(zArrow* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* call destructor of base object */
    zBase_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;
    
    if(obj->z_int_flg)
	free(obj);
}

/* Draw function */
int zArrow_Draw(zArrow* obj)
{
    #define ZARROW_ARR 3
    zBase* _base;
    zGeneric* _genric;
    cairo_t* _dev_c;
    int rt_val;
    double x1[ZARROW_ARR], y1[ZARROW_ARR], x2[ZARROW_ARR], y2[ZARROW_ARR], h, ang;
    int i;
    
    /* check for object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    _base = &obj->z_parent;
    _genric = &obj->z_parent.z_sgeneric;

    /* Device context pointer */
    _dev_c = zGeneric_Get_Dev_Context(_genric);

    /* Check all objects */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_genric);
    Z_CHECK_OBJ(_dev_c);

    /* Check if the angle is greater than zero. 
     * If the angle is greater than zero, save cairo context
     * and translate and rotate the coordinate system */
    cairo_save(_dev_c);

    cairo_translate(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS(_base->z_y));
    
    cairo_rotate(_dev_c,
		 CONV_TO_POINTS(obj->z_ang));
    
    switch(obj->z_arrow_type)
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

    x1[0] = 0.0;
    y1[0] = 0.0;
    x2[0] = -1 * h * tan(CONV_TO_RADIANS(ang));
    y2[0] = -1 * h;P

    x1[1] = x2[0];
    y1[1] = y2[0];
    x2[1] = h * tan(CONV_TO_RADIANS(ang));
    y2[1] = y1[1];

    x1[2] = x2[1];
    y1[2] = y2[1];
    x2[2] = 0;
    y2[2] = 0;

    cairo_move_to(_dev_c,
		  CONV_TO_POINTS(x1[0]),
		  CONV_TO_POINTS(y1[0]));
    for(i = 1; i < ZARROW_ARR; i++)
	{
	    cairo_line_to(_dev_c,
			  CONV_TO_POINTS(x1[i]),
			  CONV_TO_POINTS(y1[i]));
	}

    if(obj->z_fill_flg)
	cairo_fill(_dev_c);
    else
	cairo_stroke(_dev_c);

    cairo_rotate(_dev_c, 0.0);
    
    cairo_translate(_dev_c,
		    -1 * CONV_TO_POINTS(_base->z_x),
		    -1 * CONV_TO_POINTS(_base->z_y));
    
    cairo_restore(_dev_c);
    
    return 0;
}

/* Set angle */
inline int zArrow_Set_Angle(zArrow* obj, double ang)
{
    Z_CHECK_OBJ(obj);
    obj->z_ang = ang;
    return 0;
}

/* Get angle */
inline double zArrow_Get_Angle(zArrow* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_ang;
}

/* Set fill arrow head */
inline int zArrow_Set_Fill_Flag(zArrow* obj, unsigned int flg)
{
    Z_CHECK_OBJ(obj);
    obj->z_fill_flg = flg;
    return 0;
}

/* Get fill arrow head */
inline unsigned int zArrow_Get_Fill_Flag(zArrow* obj)
{
    Z_CHECK_OBJ(obj);
    return obj->z_fill_flg;
}

/* Set arrow head type */
inline int zArrow_Set_Arrow_Type(zArrow* obj, zArrowTypes type)
{
    Z_CHECK_OBJ(obj);
    obj->z_arrow_type = type;
    return 0;
}

/* Get arrow head type */
inline zArrowTypes zArrow_Get_Arrow_Type(zArrow* obj)
{
    if(obj == NULL)
	return zArrow1;
    else
	return obj->z_arrow_type;
}

/*========================================================================*/
/* Virtual function implementation */

/* Draw method */
static int _zarrow_draw_function(zGeneric* obj)
{
    int rt_val;
    zArrow za;
    Z_CHECK_OBJ(obj);
    za = Z_ARROW(obj);
    rt_val = zArrow_Draw(Z_ARROW(obj));
    if(za->z_draw_func)
	return za->z_draw_func(obj);
    else
	return rt_val;
}
