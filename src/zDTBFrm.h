/* Header file for top and bottom frame class. Inherits zDFrame class.
 * Tue Mar 13 23:09:39 GMT 2012 */

#ifndef _ZDTBFRM_H_
#define _ZDTBFRM_H_

#include "zGeneric.h"
#include "zDFrame.h"

/* Forward declaration of struct */
typedef struct _zDTBFrm zDTBFrm;

struct _zDTBFrm
{
    zDFrame z_parent;				/* inherited parent object */
    unsigned int z_int_flg;			/* internal flag */
    zgeneric_fptr z_draw_func;			/* draw function */
    void* z_child;				/* child object */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zDTBFrm_New(zDTBFrm* obj);
    void zDTBFrm_Delete(zDTBFrm* obj);

    /* draw method */
    int zDTBFrm_Draw(zDTBFrm* obj);

#ifdef __cplusplus
}
#endif


#endif /* _ZDTBFRM_H_ */
