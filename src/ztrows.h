/* zTRows class is a collection of zTRow. This class shall derive the
 * table class directly */
/* Mon Mar  5 20:58:19 GMT 2012 */

#ifndef _ZTROWS_H_
#define _ZTROWS_H_

#include <stdlib.h>
#include "zdevice.h"
#include "ztrow.h"

/* Forward declaration of struct */
typedef struct _ztrows ztrows;

struct _ztrows
{
    zgenerics parent;					/* Inherited parent object */
    unsigned int _init_flg;				/* Internal flag */
    
    struct _zgeneric_vtable vtable;			/* vtable */
    
    zgenerics* super_cls;				/* super class */
    void* child;					/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor & destructor */
    zgenerics* ztrows_new(ztrows* obj,			/* optional argument */
			  zdevice* dev,			/* device object */
			  unsigned int num_rows,	/* number of rows */
			  unsigned int num_cols,	/* number of columns */
			  double x,			/* origin x */
			  double y,			/* origin y */
			  double width,			/* width */
			  double height);		/* height */
    void ztrows_delete(ztrows* obj);

    /* Readonly property method */
    /* Get return row object of index ix */
    inline __attribute__ ((always_inline)) static ztrow* ztrows_get_row(ztrows* obj, unsigned int ix)
    {
	ZCHECK_OBJ_PTR(obj);
	if(ix >= obj->parent.count)
	    return NULL;

	return Z_TROW(obj->super_cls->generics_s[ix]);
    }
    

#ifdef __cplusplus
}
#endif

#endif /* _ZTROWS_H_ */
