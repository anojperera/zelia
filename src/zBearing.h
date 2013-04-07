/* Header file for declaring interfaces for
 * bearing class */

#ifndef _ZBEARING_H_
#define _ZBEARING_H_

#include <stdlib.h>
#include "zBase.h"

/* Foward declaration of struct */
typedef struct _zBearing zBearing;

struct _zBearing
{
    zBase z_parent;				/* parent object */
    unsigned int z_int_flg;			/* internal flag */
    void* z_child;				/* child pointer */
    zgeneric_fptr z_draw_func;			/* draw function */
    zBearingTypes z_type;			/* bearing type */
    size_t z_obj_sz;				/* object size */
};


#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor & Destructor */
    zGeneric* zBearing_New(zBearing* obj);
    void zBearing_Delete(zBearing* obj);

    /* Draw method */
    int zBearing_Draw(zBearing* obj);

    /* Set and get bearings */
    inline int zBearing_Set_Type(zBearing* obj, zBearingTypes ztype);
    inline zBearingTypes zBearing_Get_Type(zBearing* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZBEARING_H_ */
