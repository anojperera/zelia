/* Generic arrow head class, to be inherited by all
 * Mon Feb  6 21:57:45 GMT 2012 */

#ifndef _ZARROW_H_
#define _ZARROW_H_

#include <stdlib.h>
#include "zGeneric.h"
#include "zBase.h"
#include "zVar.h"

/* Forward declaration of struct */
typedef struct _zArrow zArrow;

struct _zArrow
{
    zBase z_parent;				/* inherited parent object */
    unsigned int z_int_flg;			/* internal flag */
    zArrowTypes z_arrow_type;			/* arrow head type */
    double z_ang;				/* orientation angle */
    unsigned int z_fill_flg;			/* flag to indicate fill arrow */
    zgeneric_fptr z_draw_func;			/* draw function pointer */
    void* z_child;				/* child pointer */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zGeneric* zArrow_New(zArrow* obj);
    void zArrow_Delete(zArrow* obj);

    /* Draw function */
    int zArrow_Draw(zArrow* obj);

    /*************************************************************/
    /* Property methods */

    /* Set and get orientation angle */
    inline int zArrow_Set_Angle(zArrow* obj, double ang);
    inline double zArrow_Get_Angle(zArrow* obj);

    /* Set and get fill flag */
    inline int zArrow_Set_Fill_Flag(zArrow* obj, unsigned int flg);
    inline unsigned int zArrow_Get_Fill_Flag(zArrow* obj);

    /* Set and get arrow head type */
    inline int zArrow_Set_Arrow_Type(zArrow* obj, zArrowTypes type);
    inline zArrowTypes zArrow_Get_Arrow_Type(zArrow* obj);

#ifdef __cplusplus
}
#endif    
    
#endif /* _ZARROW_H_ */
