/* zTRow class represents a collection of cells which forms a row in a
 * table. zTRow inherites zGenerics collection class and is dependant on
 * zTCell. zTRow uses a static array in zGenerics collection class therefore
 * once the number of columns are specified it cannot be changed.
 * zTRow shall derrive zTRows collection class */
/* Sun Mar  4 12:46:38 GMT 2012 */

#ifndef _ZTROW_H_
#define _ZTROW_H_

#include <stdlib.h>
#include "zGenerics.h"
#include "zDevice.h"
#include "zTCell.h"

/* Forward declaration of struct. */
typedef struct _zTRow zTRow;

struct _zTRow
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
    zGenerics* zTRow_New(zTRow* obj,		/* optional argument */
			 zDevice* dev,		/* device object */
			 unsigned int row_ix,	/* row index */
			 unsigned int num_col,	/* number of columns */
			 double x,		/* origin x */
			 double y,		/* origin y */
			 double col_width,	/* column width */
			 double row_height);	/* row height */
    void zTRow_Delete(zTRow* obj);

    /* Add content to the cell specified by column
     * index */
    inline int zTRow_Add_Content(zTRow* obj,
				 unsigned int ix,	/* Column index */
				 const char* content);	/* Content */

    /* Returns the cell specified by the column index,
     * Returns NULL on failure */
    inline zTCell* zTRow_Get_Cell(zTRow* obj,
				  unsigned int ix);
#ifdef __cplusplus
}
#endif

#endif /* _ZTROW_H_ */
