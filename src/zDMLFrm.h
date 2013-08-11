/* Header file for defining interface for mullion.
 * Inherits zDFrame class.
 * Wed Mar 14 16:11:07 GMTST 2012 */

#ifndef __ZDMLFRM_H__
#define __ZDMLFRM_H__

#include "zDFrame.h"
#include "zGeneric.h"

/* Forward declaration of struct */
typedef struct _zDMLFrm zDMLFrm;

struct _zDMLFrm
{
    zDFrame z_parent;					/* inherited parent object */
    unsigned int z_int_flg;				/* internal flag */
    unsigned int z_ml_type;				/* mullion type */
    double z_ml_return_lip;				/* mullion return lip width */
    zgeneric_fptr z_draw_func;				/* draw function */
    void* z_child;					/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zDMLFrm_New(zDMLFrm* obj);
    void zDMLFrm_Delete(zDMLFrm* obj);

    /* Draw method */
    int zDMLFrm_Draw(zDMLFrm* obj);

    /******************************************************/
    /* Property methods */
    /* Set mullion type, default is the MFD / Fire damper type.
     * Setting the value to 1 shall draw the full mullion. */
    inline int zDMLFrm_Set_Type(zDMLFrm* obj, unsigned int ztype);
    inline unsigned int zDMLFrm_Get_Type(zDMLFrm* obj);

    /* Set and get mullion return lip */
    /* Overrides default mullion width of 25.0 */
    inline int zDMLFrm_Set_Return_Lip(zDMLFrm* obj, double width);
    inline double zDMLFrm_Get_Return_Lip(zDMLFrm* obj);

#ifdef __cplusplus
}
#endif

#endif /* __ZDMLFRM_H__ */
