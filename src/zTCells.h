/* zTCells class represents a collection of cells which forms a row in a
 * table. zTCells inherites zGenerics collection class and is dependant on
 * zTCell. zTCells uses a static array in zGenerics collection class therefore
 * once the number of columns are specified it cannot be changed.
 * zTCells shall is associated with zTRow */
/* Sun Mar  4 12:46:38 GMT 2012 */

#ifndef _ZTCELLS_H_
#define _ZTCELLS_H_

#include <stdlib.h>
#include "zGenerics.h"
#include "zDevice.h"
#include "zTCell.h"

/* Forward declaration of struct. */
typedef struct _zTCells zTCells;

struct _zTCells
{
    zGenerics z_parent;				/* Inherited parent object */
    unsigned int z_int_flg;			/* Internal flag */
    unsigned int z_row_ix;			/* Row index */
    void* z_child;				/* Child pointer */
    zcollection_fptr z_draw_func;		/* Draw function pointer */
    double z_width;				/* Column width */
    double z_height;				/* Row height */
    size_t z_obj_sz;				/* Object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGenerics* zTCells_New(zTCells* obj,	/* optional argument */
			 zDevice* dev,		/* device object */
			 unsigned int row_ix,	/* row index */
			 unsigned int num_col,	/* number of columns */
			 double x,		/* origin x */
			 double y,		/* origin y */
			 double col_width,	/* column width */
			 double row_height);	/* row height */
    void zTCells_Delete(zTCells* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZTCELLS_H_ */
