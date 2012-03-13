/* Header file for Drive side frame class. Inherits zDFrame class.
 * Tue Mar 13 20:32:27 GMT 2012 */

#ifndef _ZDSIDEFRM_H_
#define _ZDSIDEFRM_H_

#include "zGeneric.h"
#include "zDFrame.h"

/* Forward declaration of struct */
typedef struct _zDSideFrm zDSideFrm;

struct _zDSideFrm
{
    zDFrame z_parent;				/* inherited parent object */
    unsigned int z_int_flg;			/* internal flag */
    zgeneric_fptr z_draw_func;			/* drawing function */
    void* z_child;				/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor & Destructor */
    zGeneric* zDSideFrm_New(zDSideFrm* obj);
    void zDSideFrm_Delete(zDSideFrm* obj);

    int zDSideFrm_Draw(zDSideFrm* obj);

#ifdef __cplusplus
}
#endif


#endif /* _ZDSIDEFRM_H_ */
