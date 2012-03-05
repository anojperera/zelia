/* zTRow class is further abstraction layer collection cells
 * cells collection class.
 * It is however implemented as a generic object rather than a
 * collection class.
 * zTRow class is associated with zTCells collection */
/* Sun Mar  4 18:00:56 GMT 2012 */

#ifndef _ZTROW_H_
#define _ZTROW_H_

#include <stdlib.h>
#include "zBase.h"
#include "zTCell.h"
#include "zTCells.h"

/* Forward declaration of struct */
typedef struct _zTRow zTRow;

struct _zTRow
{
    zBase z_parent;			/* Inherited parent object */
    zTCells z_tcells;			/* Cells collection */
    unsigned int z_int_flg;		/* Internal flag */
    unsigned int z_num_cols;		/* Number of columns */
    unsigned int z_arr_flg;		/* Array created flag */
    unsigned int z_ix;			/* Row index */
    void* z_child;			/* Child pointer */
    zgeneric_fptr z_draw_func;		/* Draw function pointer */
    size_t z_obj_sz;			/* Object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zTRow_New(zTRow* obj, unsigned int ix);
    void zTRow_Delete(zTRow* obj);

    /* Draw function */
    int zTRow_Draw(zTRow* obj);

    /* Add content to the cell specified by column
     * index */
    inline int zTRow_Add_Content(zTRow* obj,
				 unsigned int ix,	/* Column index */
				 const char* content);	/* Content */

    /* Returns the cell specified by the column index,
     * Returns NULL on failure */
    inline zTCell* zTRow_Get_Cell(zTRow* obj,
				  unsigned int ix);

    /* Property methods */
    inline int zTRow_Set_Num_Cols(zTRow* obj, unsigned int num_cols);
    inline unsigned int zTRow_Get_Num_Cols(zTRow* obj);

#ifdef __cplusplus
}
#endif    

#endif /* _ZTROW_H_ */
