/* Header file for defining interfaces for transom.
 * Inherites zDFrame class.
 * Wed Mar 14 17:40:14 GMTST 2012 */

#ifndef __ZDTRFRM_H__
#define __ZDTRFRM_H__

#include "zGeneric.h"
#include "zDFrame.h"


/* Forward declaration of struct */
typedef struct _zDTRFrm zDTRFrm;

struct _zDTRFrm
{
    zDFrame z_parent;					/* inherited parent class */
    unsigned int z_int_flg;				/* internal flag */
    double z_tr_return_lip;				/* return lip width */
    zgeneric_fptr z_draw_func;				/* draw function */
    void* z_child;					/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */

    zGeneric* zDTRFrm_New(zDTRFrm* obj);
    void zDTRFrm_Delete(zDTRFrm* obj);

    /* Draw method */
    int zDTRFrm_Draw(zDTRFrm* obj);

    /* Property method */
    /* Set and get return lip width */
    inline int zDTRFrm_Set_Return_Lip(zDTRFrm* obj, double width);
    inline double zDTRFrm_Get_Return_Lip(zDTRFrm* obj);

#ifdef __cplusplus
}
#endif

#endif /* __ZDTRFRM_H__ */
