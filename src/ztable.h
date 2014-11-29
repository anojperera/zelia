/* zTable class header file provides interfaces for producing a 2 dimensional
 * table with content. */
/* Tue Mar  6 12:44:22 GMTST 2012 */


#ifndef __ZTABLE_H__
#define __ZTABLE_H__

#include <stdlib.h>
#include "zbase.h"

#include "ztcell.h"
#include "ztrow.h"
#include "ztrows.h"

/* Forward declaration of struct */
typedef struct _ztable ztable;

struct _ztable
{
    zbase parent;					/* Inherited parent object */
    ztrows rows;					/* Rows collection */
	
    unsigned int _int_flg;				/* Internal flag */
    unsigned int arr_flg;				/* Flag to indicate array created */
    unsigned int num_rows;				/* Number of rows */
    unsigned int num_cols;				/* Number of colums */
    double* col_widths;				/* Column width array */

	struct _zgeneric_vtable vtable;
	zgeneric* super_cls;
    void* child;					/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zgeneric* ztable_new(ztable* obj);
    void ztable_delete(ztable* obj);

    /* Draw method */
    int ztable_draw(ztable* obj);

    /* Property methods */
    /*****************************************/
    /* Set and getnumber of rows and columns */
    int ztable_set_rows_and_cols(ztable* obj,
					unsigned int num_rows,
					unsigned int num_cols);
    inline unsigned int ztable_get_rows(ztable* obj);
    inline unsigned int ztable_get_cols(ztable* obj);

    /* Set content of cell spcecified by row and column index */
    inline int ztable_set_content(ztable* obj,
				  unsigned int row_ix,
				  unsigned int col_ix,
				  const char* content);
    inline const char* ztable_get_content(ztable* obj,
					  unsigned int row_ix,
					  unsigned int col_ix);

    /* Get row object specified by row index */
    inline const ztrow* ztable_get_row(ztable* obj,
				       unsigned int row_ix);
    
    /* Get cell object specified by column index */
    inline const ztcell* ztable_get_cell(ztable* obj,
					 unsigned int row_ix,
					 unsigned int col_ix);

    /* Set and get column width */
    inline int ztable_set_column_width(ztable* obj,
				       unsigned int col_ix,
				       double width);
    inline double ztable_get_column_width(ztable* obj,
					  unsigned int col_ix);
#ifdef __cplusplus
}
#endif

#endif /* __ZTABLE_H__ */
