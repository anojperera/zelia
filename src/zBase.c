/* Implementation of zBase class
 * Sat Jan 21 16:23:09 GMT 2012 */

#include "zBase.h"

/* Virtual function call back */
static int zbase_draw_function(zGeneric* obj);


/* Constructor */
zGeneric* zBase_New(zBase* obj)
{
    if(obj == NULL)
	{
	    obj = (zBase*) malloc(sizeof(zBase));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* create parent object */
    zGeneric_New(&obj->z_sgeneric);

    /* set properties */
    obj->z_x = 0.0;
    obj->z_y = 0.0;
    obj->z_ang = 0.0;
    obj->z_width = 0.0;
    obj->z_height = 0.0;
    obj->z_child = NULL;

    obj->z_draw_func = NULL;

    /* assigne function pointer of base object */
    obj->z_sgeneric.z_draw_func = zbase_draw_function;

    /* set child pointer of parent object */
    obj->z_sgeneric.z_child = (void*) obj;

    /* return base pointer */
    return &obj->z_sgeneric;
}

/* Destructor */
void zBase_Delete(zBase* obj)
{
    obj->z_draw_func = NULL;
    obj->z_child = NULL;
    zGeneric_Delete(&obj->z_sgeneric);

    if(obj->z_int_flg)
	free(obj);

    return;
}

/* Set base coordinates */
inline int zBase_Set_Base_Coords(zBase* obj, double x, double y)
{
    /* check for object pointers */
    Z_CHECK_OBJ(obj);
    obj->z_x = x;
    obj->z_y = y;
    return 0;
}

/* Set orientation angle */
inline int zBase_Set_Orientation(zBase* obj, double ang)
{
    Z_CHECK_OBJ(obj);
    obj->z_ang = ang;
    return 0;
}

/* Get orientation angle */
inline double zBase_Get_Orientation(zBase* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_ang;
}

inline int zBase_Set_Width_and_Height(zBase* obj, double width, double height)
{
    /* check for object */
    Z_CHECK_OBJ(obj);
    obj->z_width = width;
    obj->z_height = height;
    return 0;
}

/* Set width */
inline int zBase_Set_Width(zBase* obj, double width)
{
    /* check object */
    Z_CHECK_OBJ(obj);
    obj->z_width = width;
    return 0;
}

/* Get width */
inline double zBase_Get_Width(zBase* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_width;
}

/* Set height */
inline int zBase_Set_Height(zBase* obj, double height)
{
    Z_CHECK_OBJ(obj);
    obj->z_height = height;
    return 0;
}

/* Get height */
inline double zBase_Get_Height(zBase* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_height;
}

/*==================================================================*/
/* Private functions */

/* Draw function */
static int zbase_draw_function(zGeneric* obj)
{
    zBase* self;
    Z_CHECK_OBJ(obj);
    self = Z_BASE(obj);
    if(self->z_draw_func)
    	return self->z_draw_func(obj);
    else
	return 0;
}

