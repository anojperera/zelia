/* zTable class header file provides interfaces for producing a 2 dimensional
 * table with content. */
/* Tue Mar  6 12:44:22 GMTST 2012 */


#ifndef __ZTABLE_H__
#define __ZTABLE_H__

#include <stdlib.h>
#include "zBase.h"
#include "zTRows.h"
#include "zTRow.h"
#include "zTCell.h"

/* Forward declaration of struct */
typedef struct _zTable zTable;

struct _zTable
{
    zBase z_parent;					/* Inherited parent object */
    zTRows z_rows;					/* Rows collection */
    unsigned int z_int_flg;				/* Internal flag */
    unsigned int z_arr_flg;				/* Flag to indicate array created */
    unsigned int z_num_rows;				/* Number of rows */
    unsigned int z_num_cols;				/* Number of colums */
    void* z_child;					/* Child pointer */
    zgeneric_fptr z_draw_func;				/* Draw function */
    size_t z_obj_sz;					/* Object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zGeneric* zTable_New(zTable* obj);
    void zTable_Delete(zTable* obj);

    /* Draw method */
    int zTable_Draw(zTable* obj);

    /* Property methods */
    /*****************************************/
    /* Set and getnumber of rows and columns */
    inline int zTable_Set_Rows_and_Cols(zTable* obj,
					unsigned int num_rows,
					unsigned int num_cols);
    inline unsigned int zTable_Get_Rows(zTable* obj);
    inline unsigned int zTable_Get_Cols(zTable* obj);

    /* Set content of cell spcecified by row and column index */
    inline int zTable_Set_Content(zTable* obj,
				  unsigned int row_ix,
				  unsigned int col_ix,
				  const char* content);
    inline const char* zTable_Get_Content(zTable* obj,
					  unsigned int row_ix,
					  unsigned int col_ix);

    /* Get row object specified by row index */
    inline const zTRow* zTable_Get_Row(zTable* obj,
				       unsigned int row_ix);
    
    /* Get cell object specified by column index */
    inline const zTCell* zTable_Get_Cell(zTable* obj,
					 unsigned int row_ix,
					 unsigned int col_ix);
#ifdef __cplusplus
}
#endif

#endif /* __ZTABLE_H__ */
