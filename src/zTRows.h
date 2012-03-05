/* zTRows class is a collection of zTRow. This class shall derive the
 * table class directly */
/* Mon Mar  5 20:58:19 GMT 2012 */

#ifndef _ZTROWS_H_
#define _ZTROWS_H_

#include <stdlib.h>
#include "zDevice.h"
#include "zTRow.h"

/* Forward declaration of struct */
typedef struct _zTRows zTRows;

struct _zTRows
{
    zGenerics z_parent;				/* Inherited parent object */
    unsigned int z_int_flg;			/* Internal flag */
    void* z_child;				/* Child pointer */
    zcollection_fptr z_draw_func;		/* Draw function */
    size_t z_obj_sz;				/* Object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor & destructor */
    zGenerics* zTRows_New(zTRows* obj,			/* optional argument */
			  zDevice* dev,			/* device object */
			  unsigned int num_rows,	/* number of rows */
			  double x,			/* origin x */
			  double y,			/* origin y */
			  double width,			/* width */
			  double height);		/* height */
    void zTRows_Delete(zTRows* obj);

    /* Readonly property method */
    /* Get return row object of index ix */
    zTRow* zTRows_Get_Row(zTRows* obj, unsigned int ix);
    

#ifdef __cplusplus
}
#endif

#endif /* _ZTROWS_H_ */
