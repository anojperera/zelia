/* ztcells class represents a collection of cells which forms a row in a
 * table. ztcells inherites zGenerics collection class and is dependant on
 * ztcell. ztcells uses a static array in zGenerics collection class therefore
 * once the number of columns are specified it cannot be changed.
 * ztcells shall is associated with zTRow */
/* Sun Mar  4 12:46:38 GMT 2012 */

#ifndef _ZTCELLS_H_
#define _ZTCELLS_H_

#include <stdlib.h>
#include "zgenerics.h"
#include "zdevice.h"
#include "ztcell.h"

/* Forward declaration of struct. */
typedef struct _ztcells ztcells;

struct _ztcells
{
    zgenerics parent;					/* Inherited parent object */
    unsigned int _init_flg;				/* Internal flag */
    unsigned int row_ix;				/* Row index */

    double width;					/* Column width */
    double height;					/* Row height */

    struct _zgeneric_vtable vtable;			/* vtable for delete, draw and new function pointers */

    zgenerics* super_cls;				/* super class pointer */
    void* child;					/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgenerics* ztcells_new(ztcells* obj,		/* optional argument */
			   zdevice* dev,		/* device object */
			   unsigned int row_ix,		/* row index */
			   unsigned int num_col,	/* number of columns */
			   double x,			/* origin x */
			   double y,			/* origin y */
			   double col_width,		/* column width */
			   double row_height);		/* row height */

    void ztcells_delete(ztcells* obj);

    /* Get cell specified by the index */
    inline __attribute__ ((always_inline)) static ztcell* ztcells_get_cell(ztcells* obj, unsigned int ix)
    {
	ZCHECK_OBJ_PTR(obj);

	if(ix >= obj->z_parent.z_count)
	    return NULL;

	return Z_TCELL(obj->parent.generics_s[ix]);
    }

#ifdef __cplusplus
}
#endif

#endif /* _ZTCELLS_H_ */
