/* zTRow class is further abstraction layer collection cells
 * cells collection class.
 * It is however implemented as a generic object rather than a
 * collection class.
 * zTRow class is associated with zTCells collection */
/* Sun Mar  4 18:00:56 GMT 2012 */

#ifndef _ZTROW_H_
#define _ZTROW_H_

#include <stdlib.h>
#include "zbase.h"
#include "ztcell.h"
#include "ztcells.h"

/* Forward declaration of struct */
typedef struct _ztrow ztrow;

struct _ztrow
{
    zbase parent;								/* inherited parent object */
    ztcells tcells;								/* Cells collection */
    unsigned int int_flg;						/* Internal flag */
    unsigned int num_cols;					   	/* Number of columns */
    unsigned int arr_flg;						/* Array created flag */
    unsigned int ix;							/* Row index */

	struct _zgeneric_vtable vtable;				/* vtable */
	zgeneric* super_cls;						/* super class  pointer */
    void* child;								/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgeneric* ztrow_new(ztrow* obj, unsigned int ix);
    void ztrow_delete(ztrow* obj);

    /* Draw function */
    int ztrow_draw(ztrow* obj);

    /* Add content to the cell specified by column index */
    int ztrow_add_content(ztrow* obj,
				 unsigned int ix,				/* Column index */
				 const char* content);			/* Content */

    /* Returns the cell specified by the column index,
     * Returns NULL on failure */
    ztcell* ztrow_get_cell(ztrow* obj, unsigned int ix);

    /* Property methods */
    inline __attribute__ ((always_inline)) static int ztrow_set_num_cols(ztrow* obj, unsigned int num_cols)
	{
		ZCHECK_OBJ_INT(obj);
		obj->num_cols = num_cols;
		return ZELIA_OK;
	}
    inline __attribute__ ((always_inline)) static unsigned int ztrow_get_num_cols(ztrow* obj)
	{
		if(obj == NULL)
			return 0;
		else
			return obj->num_cols;
	}

#ifdef __cplusplus
}
#endif    

#endif /* _ZTROW_H_ */
