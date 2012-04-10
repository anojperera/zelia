/* Header file for shaft interfaces.
 * Inherits base classe.
 * Tue Apr 10 21:55:11 BST 2012 */

#ifndef _ZSHAFT_H_
#define _ZSHAFT_H_

#include <stdlib.h>
#include "zGeneric.h"
#include "zBase.h"

/* Forward declaration of stuct */
typedef struct _zShaft zShaft;

struct _zShaft
{
    zBase z_parent;				/* Parent object */
    unsigned int z_int_flg;			/* Internal flag */
    zgeneric_fptr z_draw_func;			/* draw function pointer */
    void* z_child;				/* child pointer */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif
    /* Constructor and destructor */
    zGeneric* zShaft_New(zShaft* obj);
    void zShaft_Delete(zShaft* obj);

    /* Drawing method */
    int zShaft_Draw(zShaft* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZSHAFT_H_ */
