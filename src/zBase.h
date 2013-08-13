/* Base object for deriving all component object,
 * Describes, primitive coordinates */
/* Sat Jan 21 16:05:52 GMT 2012 */

#ifndef _ZBASE_H_
#define _ZBASE_H_

#include <stdlib.h>
#include "zGeneric.h"

typedef struct _zBase zBase;

struct _zBase
{
    zGeneric z_sgeneric;			/* inherited parent object */
    unsigned int z_int_flg;			/* internal flag */
    void* z_child;				/* child pointer */
    /* base coordinates */
    double z_x;
    double z_y;
    double z_ang;				/* orientation angle */
    double z_width;				/* with */
    double z_height;				/* height */
    double z_thk;				/* thickness of material */
    unsigned int z_prj_flg;			/* projected flag */
    zgeneric_fptr z_draw_func;			/* draw function */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zBase_New(zBase* obj);
    void zBase_Delete(zBase* obj);

    /* Property functions */
    /*************************************************************/

    /* Set base coordinates */
    inline __attribute__ ((always_inline)) static int zBase_Set_Base_Coords(zBase* obj, double x, double y)
    {
	/* check for object pointers */
	Z_CHECK_OBJ(obj);
	obj->z_x = x;
	obj->z_y = y;
	return 0;	
    }

    /* Set orientation angle */
    inline __attribute__ ((always_inline)) static int zBase_Set_Orientation(zBase* obj, double ang)
    {
	Z_CHECK_OBJ(obj);
	obj->z_ang = ang;
	return 0;
    }
    inline __attribute__ ((always_inline)) static double zBase_Get_Orientation(zBase* obj)
    {
	Z_CHECK_OBJ_DOUBLE(obj);
	return obj->z_ang;
    }

    /* Set and Get width height */
    inline __attribute__ ((always_inline)) static int zBase_Set_Width_and_Height(zBase* obj, double width, double height)
    {
	/* check for object */
	Z_CHECK_OBJ(obj);
	obj->z_width = width;
	obj->z_height = height;
	return 0;
    }
    
    inline __attribute__ ((always_inline)) static int zBase_Set_Width(zBase* obj, double width)
    {
	/* check object */
	Z_CHECK_OBJ(obj);
	obj->z_width = width;
	return 0;
    }
    
    inline __attribute__ ((always_inline)) static double zBase_Get_Width(zBase* obj)
    {
	Z_CHECK_OBJ_DOUBLE(obj);
	return obj->z_width;
    }

    inline __attribute__ ((always_inline)) static int zBase_Set_Height(zBase* obj, double height)
    {
	Z_CHECK_OBJ(obj);
	obj->z_height = height;
	return 0;
    }
    inline __attribute__ ((always_inline)) static double zBase_Get_Height(zBase* obj)
    {
	Z_CHECK_OBJ_DOUBLE(obj);
	return obj->z_height;
    }

    inline __attribute__ ((always_inline)) static int zBase_Set_Thickness(zBase* obj, double thk)
    {
	Z_CHECK_OBJ(obj);
	obj->z_thk = thk;
	return 0;
    }
    inline __attribute__ ((always_inline)) static double zBase_Get_Thickness(zBase* obj)
    {
	Z_CHECK_OBJ_DOUBLE(obj);
	return obj->z_thk;
    }

    inline __attribute__ ((always_inline)) static int zBase_Set_Projected_Flag(zBase* obj, unsigned int flg)
    {
	Z_CHECK_OBJ(obj);
	obj->z_prj_flg = flg;
	return 0;
    }
    inline __attribute__ ((always_inline)) static unsigned int zBase_Get_Projected_Flag(zBase* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->z_prj_flg;
    }
    
#ifdef __cplusplus
}
#endif

#endif /* _ZBASE_H_ */
