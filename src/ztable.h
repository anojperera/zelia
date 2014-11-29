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
    double* col_widths;					/* Column width array */

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
    inline __attributes__ ((always_inline)) static unsigned int ztable_get_rows(ztable* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->num_rows;
    }

    inline __attributes__ ((always_inline)) static unsigned int ztable_get_cols(ztable* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->num_cols;
    }

    /* Set content of cell spcecified by row and column index */
    int ztable_set_content(ztable* obj,
			   unsigned int row_ix,
			   unsigned int col_ix,
			   const char* content);
    const char* ztable_get_content(ztable* obj,
				   unsigned int row_ix,
				   unsigned int col_ix);

    /* Get row object specified by row index */
    inline __attribute__ ((always_inline)) static const ztrow* ztable_get_row(ztable* obj, unsigned int row_ix)
    {
	/* check row object */
	ZCHECK_OBJ_PTR(obj);

	/* check number of rows */
	if(row_ix > obj->num_rows)
	    return NULL;

	return ztrows_get_row(&obj->rows, row_ix);
    }

    /* Get cell object specified by column index */
    const ztcell* ztable_get_cell(ztable* obj,
				  unsigned int row_ix,
				  unsigned int col_ix);

    /* Set and get column width */
    inline int ztable_set_column_width(ztable* obj,
				       unsigned int col_ix,
				       double width);
    inline __attribute__ ((always_inline)) static  double ztable_get_column_width(ztable* obj,
										  unsigned int col_ix)
    {
	/* check object */
	ZCHECK_OBJ_DOUBLE(obj);

	/* check if column width array was created */
	ZCHECK_OBJ_DOUBLE(obj->col_widths);

	/* check array bounds */
	if(col_ix >= obj->num_cols)
	    return 0.0;

	return obj->col_widths[col_ix];
    }
#ifdef __cplusplus
}
#endif

#endif /* __ZTABLE_H__ */
