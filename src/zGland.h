/* Header file for describing interfaces for cable glands
 * inherits base object.
 * Fri Feb  3 13:40:12 GMTST 2012 */

#ifndef __ZGLAND_H__
#define __ZGLAND_H__

#include "zDevice.h"
#include "zGeneric.h"
#include "zBase.h"

/* Forward declaration of struct */
typedef	struct _zGland zGland;

struct _zGland
{
    zBase z_parent;				/* inherited parent object */
    unsigned int z_int_flg;			/* internal flag */
    double z_dia;				/* diameter */
    zGlandSize z_gland_size;			/* gland size */
    zgeneric_fptr z_draw_func;			/* draw function pointer */
    void* z_child;				/* child object */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zGland_New(zGland* obj,		/* optional object */
			 zDevice* dev,		/* optional device object */
			 zGlandSize sz);	/* size parameter */

    void zGland_Delete(zGland* obj);


    /* Draw method */
    int zGland_Draw(zGland* obj);

    /************************************************/
    /* Property methods */

    /* Set and get gland diameter */
    inline int zGland_Set_Gland_Dia(zGland* obj, double dia);
    inline double zGland_Get_Gland_Dia(zGland* obj);

    /* Set and get gland size */
    inline int zGland_Set_Gland_Size(zGland* obj, zGlandSize sz);
    inline zGlandSize zGland_Set_Gland_Size(zGland* obj);


#ifdef __cplusplus
}
#endif
    
#endif /* __ZGLAND_H__ */
