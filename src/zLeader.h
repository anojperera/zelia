/* Header file describes interfaces for creating a leader.
 * Inherits arrow class. 256 char buffer is provided for
 * storing a description for the leader.
 * Tue Feb  7 15:50:50 GMTST 2012 */

#ifndef __ZLEADER_H__
#define __ZLEADER_H__

#include <stdlib.h>
#include "zArrow.h"
#include "zGeneric.h"
#include "zDevice.h"

#define Z_LEADER_DESC_BUFF 256

/* Forward declaration of struct */
typedef struct _zLeader zLeader;

struct _zLeader
{
    zArrow z_parent;						/* inherited parent object */
    unsigned int z_int_flg;					/* internal flag */
    double z_length_1;						/* first segment length */
    double z_length_2;						/* second segment length */
    zgeneric_fptr z_draw_func;					/* draw function pointer */
    void* z_child;						/* child pointer */
    size_t z_obj_sz;						/* object size */
    char z_content[Z_LEADER_DESC_BUFF];				/* leader description / content */
};

#ifdef __cplusplus
extern "C" {
#endif

    zGeneric* zLeader_New(zLeader* obj,				/* leader object optional */
			  zDevice* dev,				/* device object optional */
			  double x,			        /* base coordinate x */
			  double y,				/* base coordinate y */
			  double seg_1,				/* first segment length */
			  double seg_2,				/* second segment length */
			  double ang,				/* orientation angle */
			  const char* desc);			/* description */

    void zLeader_Delete(zLeader* obj);

    /* Draw function */
    int zLeader_Draw(zLeader* obj);

    /**************************************************************************************/
    /* Property methods */

    /* Set and get segment lengths */
    inline int zLeader_Set_Segment_Lengths(zLeader* obj, double seg_1, double seg_2);
    inline double zLeader_Get_First_Segment(zLeader* obj); 
    inline double zLeader_Get_Second_Segment(zLeader* obj);

    /* Set and get description */
    inline int zLeader_Set_Description(zLeader* obj, const char* desc);
    inline const char* zLeader_Get_Description(zLeader* obj);
#ifdef __cplusplus
}
#endif

#endif /* __ZLEADER_H__ */
