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
    void* var_child;				/* child pointer */
    /* base coordinates */
    double z_x;
    double z_y;
    double z_ang;				/* orientation angle */
    double z_width;				/* with */
    double z_height;				/* height */

    zgeneric_fptr z_draw_func;			/* draw function */
};

#ifdef __cplusplus
extern "C" {
#ifdef

    /* Constructor and destructor */
    zGeneric* zBase_New(zBase* obj);
    void zBase_Delete(zBase* obj);

    /* Property functions */
    /*************************************************************/

    /* Set base coordinates */
    inline int zBase_Set_Base_Coords(zBase* obj, double x, double y);

    /* Set orientation angle */
    inline int zBase_Set_Orientation(zBase* obj, double ang);
    inline double zBase_Get_Orientation(zBase* obj);

    /* Set and Get width height */
    inline int zBase_Set_Width(zBase* obj, double width);
    inline double zBase_Get_Width(zBase* obj);

    inline int zBase_Get_Height(zBase* obj, double height);
    inline double zBase_Get_Height(zBase* obj);
    

#ifdef __cplusplus
}
#ifdef

#endif /* _ZBASE_H_ */
