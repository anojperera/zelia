/* Header file for defining interfaces for shafts collection
 * Wed Apr 11 23:22:10 BST 2012 */

#ifndef _ZSHAFTS_H_
#define _ZSHAFTS_H_

#include <stdlib.h>
#include "zGenerics.h"
#include "zDevice.h"

/* Forward declaration of struct */
typedef struct _zShafts zShafts;

struct _zShafts
{
    zGenerics z_parent;				/* parent object */
    unsigned int z_int_flg;			/* internal flag */
    void* z_child;				/* child pointer */
    zcollection_fptr z_draw_func;		/* draw function pointer */
    double z_x;					/* x coordinate of shaft left edge */
    double z_y;					/* first shaft center position */
    double z_width;				/* shaft width */
    double z_dia;				/* shaft dia */
    double z_pitch;				/* shaft pitch */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor & destructor */
    zGenerics* zShafts_New(zShafts* obj,		/* optional self pointer */
			   zDevice* dev,		/* device context */
			   double x,			/* x coordinate */
			   double y,			/* y coordinate */
			   double width,		/* shaft width */
			   double dia,			/* shaft diameter */
			   unsigned int num_shafts,	/* number of shafts */
			   double pitch);		/* blade pitch */
    void zShafts_Delete(zShafts* obj);

    /* Set coordinates */
    inline int zShafts_Set_Coordinates(zShafts* obj, double x, double y);

#ifdef __cplusplus
}
#endif

#endif /* _ZSHAFTS_H_ */
