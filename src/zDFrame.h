/* Generic damper frame class. Provides interfaces child
 * side, top and bottom, mullion and transom frames.
 * This class shall be used for deriving the above child
 * classes. Inherits base class.
 * Sun Mar 11 13:34:18 GMT 2012 */

#ifndef _ZDFRAME_H_
#define _ZDFRAME_H_

#include <stdlib.h>
#include "zGeneric.h"
#include "zBase.h"

/* Forward declaration of struct */
typedef struct _zDFrame zDFrame;

struct _zDFrame
{
    zBase z_parent;				/* Inherited parent class */
    unsigned int z_int_flg;			/* Internal flag */
    unsigned int z_rtlip_flg;			/* Flag to indicate return lip */
    unsigned int z_frm_type;			/* Frame type - FD or ND
    						 * 0 - FD frame
						 * 1 - ND frame */
    
    unsigned int z_fillet_flg;			/* Flag to indicate fillet the frame */
    double z_rtlip_width;			/* Return lip width */
    zgeneric_fptr z_draw_func;			/* Draw function */
    void* z_child;				/* Child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zGeneric* zDFrame_New(zDFrame* obj);
    void zDFrame_Delete(zDFrame* obj);

    /* Draw methods to be defined in child classes */

    /***********************************************/
    /* Property methods */

    /* Set and get return lip flag */
    inline int zDFrame_Set_Return_Lip_Flg(zDFrame* obj, unsigned int flg);
    inline unsigned int zDFrame_Get_Return_Lip_Flg(zDFrame* obj);

    /* Set and get frame type */
    inline int zDFrame_Set_Frame_Type(zDFrame* obj, unsigned int ztype);
    inline unsigned int zDFrame_Get_Frame_Type(zDFrame* obj);

    /* Set and get fillet type flag */
    inline int zDFrame_Set_Fillet_Flg(zDFrame* obj, unsigned int flg);
    inline unsigned int zDFrame_Get_Fillet_Flg(zDFrame* obj);

    /* Set and get return lip flag */
    inline int zDFrame_Set_Return_Lip_Width(zDFrame* obj, double width);
    inline double zDFrame_Get_Return_Lip_Width(zDFrame* obj);
#ifdef __cplusplus
}
#endif    

#endif /* _ZDFRAME_H_ */
