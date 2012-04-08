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
#include "zBlades.h"

/* Forward declaration of struct */
typedef struct _zDamper zDamper;

struct _zDamper
{
    zBase z_parent;					/* inherited parent class */
    unsigned int z_int_flg;				/* internal flag */
    unsigned int z_frm_type;				/* frame type flag;
							 * 0 - FD type
							 * 1 - RD type */
    zDSideFrm z_lh_frm;					/* left hand side frame */
    zDSideFrm z_rh_frm;					/* right hand side frame */
    zDTBFrm z_t_frm;					/* top frame */
    zDTBFrm z_b_frm;					/* bottom frame */
    zBlades z_blds;					/* blades collection */
    zDMLFrm* z_mls;					/* mullion collection */
    zDTRFrm* z_trs;					/* transom collection */
    unsigned int z_num_ml;				/* number of mullions */
    unsigned int z_num_tr;				/* number of transoms */
    unsigned int z_bld_flg;				/* flag to indicate blade creation flag */
    unsigned int z_num_blades;				/* number of blades */
    zBladeType z_bld_type;				/* blade type */
    double z_oflange;					/* other flanges */
    double z_dflange;					/* drive flanges */
    zgeneric_fptr z_draw_func;				/* draw function */
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
    /* Set and get frame type */
    inline int zDamper_Set_Frame_Type(zDamper* obj, unsigned int ztype);
    inline unsigned int zDamper_Get_Frame_Type(zDamper* obj);

    /* Get frames */
    inline zDSideFrm* zDamper_Get_LH_Frame(zDamper* obj);
    inline zDSideFrm* zDamper_Get_RH_Frame(zDamper* obj);
    inline zDTBFrm* zDamper_Get_T_Frame(zDamper* obj);
    inline zDTBFrm* zDamper_Get_B_Frame(zDamper* obj);

    /* Set and get drive flange */
    inline int zDamper_Set_Drive_Flagne(zDamper* obj, double flange);
    inline double zDamper_Get_Drive_Flange(zDamper* obj);

    /* Set and get non drive flange */
    inline int zDamper_Set_NonDrive_Flange(zDamper* obj, double flange);
    inline double zDamper_Get_NonDrive_Flange(zDamper* obj);

    /* Set number of mullions */
    inline int zDamper_Set_Num_Mullions(zDamper* obj, unsigned int num);

    /* Set number of transoms */
    inline int zDamper_Set_Num_Transoms(zDamper* obj, unsigned int num);

    /* Get mullion and transom */
    inline zDMLFrm* zDamper_Get_Mullion(zDamper* obj, unsigned int ix);
    inline zDTRFrm* zDamper_Get_Transom(zDamper* obj, unsigned int ix);

    /* Get blades collection */
    inline zDamper_Set_Num_Blades(zDamper* obj, unsigned int num_blades);
    inline zDamper_Set_Blade_Type(zDamper* obj, zBladeType ztype);
    inline zBlades* zDamper_Get_Blades(zDamper* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZDAMPER_H_ */
