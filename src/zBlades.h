/* Header file for providing interfaces for blades collection.
 * Inherits the zGenerics collection and uses a static array.
 * When the constructor is called number of blades should be
 * passed.
 * Wed Mar 28 21:20:50 BST 2012 */
#ifndef _ZBLADES_H_
#define _ZBLADES_H_

#include <stdlib.h>
#include "zDevice.h"
#include "zGenerics.h"
#include "zBlade.h"

typedef struct _zBlades zBlades;

struct _zBlades
{
    zGenerics z_parent;					/* inherited parent object */
    unsigned int z_int_flg;				/* internal flag */
    double z_x;						/* x coordinate */
    double z_y;						/* y coordinate */
    void* z_child;					/* child pointer */
    zcollection_fptr z_draw_func;			/* draw function pointer */
    size_t z_obj_sz;					/* size of object */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor & destructor */
    zGenerics* zBlades_New(zBlades* obj,		/* optional object */
			   zDevice* dev,		/* device pointer */
			   double x,			/* x coordinate of blade collection */
			   double y,			/* y coordinate of blade collection */
			   double width,		/* total width of blades */
			   double height,		/* total height of blades */
			   unsigned int num_blades,	/* number of blades */
			   zBladeType ztype);
    void zBlades_Delete(zBlades* obj);
    
#ifdef __cplusplus
}
#endif

#endif /* _ZBLADES_H_ */
