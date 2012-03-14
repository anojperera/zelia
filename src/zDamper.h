/* Header file defining interfaces for damper class.
 * Inherits zBase class and is associated with zDSideFrm
 * and zDTBFrm, zDMLFrm and zDTRFrm classes.
 * The damper class does not provide direct interfaces for
 * manipulating associated frame classes however read
 * only properties methods are provided for customising
 * individual frames. */
#ifndef _ZDAMPER_H_
#define _ZDAMPER_H_

#include "zGeneric.h"
#include "zBase.h"
#include "zDSideFrm.h"
#include "zDTBFrm.h"
#include "zDTRFrm.h"
#include "zDMLFrm.h"

/* Forward declaration of struct */
typedef struct _zDamper zDamper;

struct _zDamper
{
    zBase z_parent;					/* inherited parent class */
    unsigned int z_int_flg;				/* internal flag */
    zDSideFrm z_lh_frm;					/* left hand side frame */
    zDSideFrm z_rh_frm;					/* right hand side frame */
    zDTBFrm z_t_frm;					/* top frame */
    zDTBFrm z_b_frm;					/* bottom frame */
    zDMLFrm* z_mls;					/* mullion collection */
    zDTRFrm* z_trs;					/* transom collection */
    unsigned int z_num_ml;				/* number of mullions */
    unsigned int z_num_tr;				/* number of transoms */
    zgeneric_fptr z_draw_func;				/* draw function */
    double z_oflange;					/* other flanges */
    double z_dflange;					/* drive flanges */
    void* z_child;					/* child pointer */
};


#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor & Destructor */
    zGeneric* zDamper_New(zDamper* obj);
    void zDamper_Delete(zDamper* obj);

    /* Draw method */
    int zDamper_Draw(zDamper* obj);

    /*********************************************************/
    /* Property methods */

    /* Get frames */
    inline zDSideFrm* zDamper_Get_LH_Frame(zDamper* obj);
    inline zDSideFrm* zDamper_Get_RH_Frame(zDamper* obj);
    inline zDTBFrm* zDamper_Get_T_Frame(zDamper* obj);
    inline zDTBFrm* zDamper_Get_B_Frame(zDamper* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZDAMPER_H_ */
