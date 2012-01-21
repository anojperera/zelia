/* Header file describes interfaces for terminal types */
/* Sat Jan 21 15:48:42 GMT 2012 */

#ifndef _ZTERMINAL_H_
#define _ZTERMINAL_H_

#include <stdlib.h>
#include "zGeneric"
#include "zBase.h"


/* Forward declaration of struct */
typedef struct _zTerminal zTerminal;

struct _zTerminal
{
    zBase z_sbase;			/* parent object */
    unsigned int z_int_flg;		/* internal flag */
    zTerminalType z_term_type;		/* terminal type */
    double z_term_height;		/* terminal height - projected */
    zgeneric_fptr z_draw_func;		/* draw function pointer */
    char z_term_num[5];			/* terminal number */
    int z_term_cnt;			/* terminal counter */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Consructor and Destructor */
    zGeneric* zTerminal_New(zTerminal* obj);
    void zTerminal_Delete(zTerminal* obj);

    /* Draw function */
    int zTerminal_Draw(zTerminal* obj);

    /* Property function */
    /*****************************************/

    /* Set and get terminal number */
    inline int zTerminal_Set_Terminal_Number(zTerminal* obj, int num);
    inline int zTerminal_Get_Terminal_Number(zTerminal* obj);

    /* Set and get terminal type */
    inline int zTerminal_Set_Type(zTerminal* obj, zTerminalType type);
    inline zTerminalType zTerminal_Get_Type(zTerminal* obj);

    /* Set and get projected height */
    inline int zTerminal_Set_Projected_Height(zTerminal* obj, double height);
    inline double zTerminal_Get_Projected_Height(zTerminal* obj);

#ifdef __cplusplus
}
#endif    
#endif /* _ZTERMINAL_H_ */
