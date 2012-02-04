/* Header file describes interfaces for table column.
 * Tale column to be inherited by table class */
/* Sat Feb  4 21:32:12 GMT 2012 */

#ifndef _ZTCOLUMN_H_
#define _ZTCOLUMN_H_

#include "zBase.h"
#include <stdlib.h>

/* Forward declaration of struct */
typedef struct _zTColumn zTColumn;

struct _zTColumn
{
    zBase z_parent;				/* inherited parent object */
    unsigned int z_int_flg;			/* internal flag */
    void* z_child;				/* child pointer */
    zgeneric_fptr z_draw_func;			/* draw function pointer */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zTColumn_New(zTColumn* obj);
    void zTColumn_Delete(zTColumn* obj);

    /* Draw method */
    int zTColumn_Draw(zTColumn* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZTCOLUMN_H_ */
