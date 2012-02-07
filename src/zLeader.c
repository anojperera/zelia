/* Implementation of zLeader class
 * Tue Feb  7 16:11:53 GMTST 2012 */

#include <string.h>
#include <math.h>
#include "zLeader.h"

/* Virtual draw function */
static int _zleader_draw(zGeneric* obj);


/* Constructor */
zGeneric* zLeader_New(zLeader* obj,				/* leader object optional */
		      zDevice* dev,				/* device object optional */
		      double x,				        /* base coordinate x */
		      double y,					/* base coordinate y */
		      double seg_1,				/* first segment length */
		      double seg_2,				/* second segment length */
		      double ang,				/* orientation angle */
		      const char* desc)				/* description */
{
    zGeneric* zg;
    /* check for object */
    if(obj == NULL)
	{
	    obj = (zLeader*) malloc(sizeof(zLeader));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* create parent object */
    if(!(zg = zArrow_New(&obj->z_parent)))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* if dev object is set make top level default */
    if(dev)
	{
	    zGeneric_Set_Device(zg, dev);
	    zGeneric_Set_Default_Dev_Context(zg);
	}

    /* set properties */
    zBase_Set_Base_Coords(Z_BASE(zg), x, y);
    zBase_Set_Orientation(Z_BASE(zg), ang);
    obj->z_length_1 = seg_1;
    obj->z_length_2 = seg_2;

    if(desc)
	strcpy(obj->z_content, desc);
    else
	obj->z_content[0] = '\0';

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* set function pointer of parent object */
    obj->z_parent.z_draw_func = _zleader_draw;

    /* set child pointer of parent object */
    obj->z_parent.z_child = (void*) obj;

    return zg;
}

/* Destructor */
void zLeader_Delete(zLeader* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* delete parent object */
    zArrow_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zLeader_Draw(zLeader* obj)
{
    #define ZLEADER_ARR 3
    zBase* _base;
    zGeneric* _generic;
    cairo_t* _dev_c;
    double x[ZLEADER_ARR], y[ZLEADER_ARR], ang;
    /* PangoLayout* layout;				/\* pango layout *\/ */
    /* PangoFontDescription* desc;				/\* font description *\/ */

    /* check for object */
    Z_CHECK_OBJ(obj);

    /* get base and generic object */
    _base = &obj->z_parent.z_parent;
    _generic = &obj->z_parent.z_parent.z_sgeneric;

    /* Device context */
    _dev_c = zGeneric_Get_Dev_Context(_generic);

    /* Check all objects */
    Z_CHECK_OBJ(_base);
    Z_CHECK_OBJ(_generic);
    Z_CHECK_OBJ(_dev_c);

    /* Check if the angle is greater than zero. 
     * If the angle is greater than zero, save cairo context
     * and translate and rotate the coordinate system */
    cairo_save(_dev_c);

    cairo_translate(_dev_c,
		    CONV_TO_POINTS(_base->z_x),
		    CONV_TO_POINTS(_base->z_y));

    if(_base->z_ang > 0.0)
	{
	    cairo_rotate(_dev_c,
			 CONV_TO_RADIANS(_base->z_ang));
	}
    ang = 90.0 - _base->z_ang;
    x[0] = 0.0;
    y[0] = 0.0;

    x[1] = 0.0;
    y[1] = -1 * obj->z_length_1;

    x[2] = obj->z_length_2 * sin(CONV_TO_RADIANS(ang));
    y[2] = y[1] + -1 * obj->z_length_2 * cos(CONV_TO_RADIANS(ang));

    cairo_move_to(_dev_c,
		  CONV_TO_POINTS(x[0]),
		  CONV_TO_POINTS(y[0]));
    
    cairo_line_to(_dev_c,
		  CONV_TO_POINTS(x[1]),
		  CONV_TO_POINTS(y[1]));

    cairo_line_to(_dev_c,
		  CONV_TO_POINTS(x[2]),
		  CONV_TO_POINTS(y[2]));

    cairo_stroke(_dev_c);

    /* rotate and translate back */
    if(_base->z_ang > 0.0)
	cairo_rotate(_dev_c, 0.0);
    
    cairo_translate(_dev_c,
		    -1 * CONV_TO_POINTS(_base->z_x),
		    -1 * CONV_TO_POINTS(_base->z_y));

    cairo_restore(_dev_c);

    return 0;
    
}

/* Set segment length */
inline int zLeader_Set_Segment_Lengths(zLeader* obj, double seg_1, double seg_2)
{
    Z_CHECK_OBJ(obj);
    obj->z_length_1 = seg_1;
    obj->z_length_1 = seg_2;

    return 0;
}

/* Get segment length 1 */
inline double zLeader_Get_First_Segment(zLeader* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_length_1;
}

/* Get segment length 2 */
inline double zLeader_Get_Second_Segment(zLeader* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_length_2;
}

/* Set description */
inline int zLeader_Set_Description(zLeader* obj, const char* desc)
{
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(desc);

    strcpy(obj->z_content, desc);
    return 0;
}

/* Get description */
inline const char* zLeader_Get_Description(zLeader* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_content;
}

/*===================================================================================*/
/* Virtual function */

/* Virtual draw method */
static int _zleader_draw(zGeneric* obj)
{
    zLeader* zl;
    int rt_val;
    
    Z_CHECK_OBJ(obj);
    rt_val = zLeader_Draw(Z_LEADER(obj));
    zl = Z_LEADER(obj);
    if(zl->z_draw_func)
	return zl->z_draw_func(obj);
    else
	return rt_val;
}
