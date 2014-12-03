/* Header file describes interfaces for terminal types */
/* Sat Jan 21 15:48:42 GMT 2012 */

#ifndef _ZTERMINAL_H_
#define _ZTERMINAL_H_

#include <stdlib.h>
#include "zgeneric.h"
#include "zbase.h"


/* Forward declaration of struct */
typedef struct _zterminal zterminal;

struct _zterminal
{
    zbase parent;			/* parent class */
    unsigned int _init_flg;		/* internal flag */

    zTerminalType term_type;		/* terminal type */

    double term_height;			/* terminal height - projected */

    char term_num[5];			/* terminal number */
    int term_cnt;			/* terminal counter */

    struct _zgeneric_vtable vtable;	/* vtable */
    zgeneric* super_cls;		/* super class pointer */
    void* child;			/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Consructor and Destructor */
    zgeneric* zterminal_new(zterminal* obj);
    void zterminal_delete(zterminal* obj);

    /* Draw function */
    int zterminal_draw(zterminal* obj);

    /* Property function */
    /*****************************************/

    /* Set and get terminal number */
    inline __attribute__ ((always_inline)) static int zterminal_set_terminal_number(zterminal* obj, int num)
    {
	ZCHECK_OBJ_INT(obj);
	sprintf(obj->term_num, "%i", num);
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static int zterminal_get_terminal_number(zterminal* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->term_cnt;
    }

    /* Set and get terminal type */
    inline __attribute__ ((always_inline)) static int zterminal_set_type(zterminal* obj, zTerminalType type)
    {
	ZCHECK_OBJ_INT(obj);
	obj->term_type = type;
	return 0;
    }
    inline __attribute__ ((always_inline)) static zTerminalType zterminal_get_type(zterminal* obj)
    {
	ZCHECK_OBJ_INT(obj);
	return obj->term_type;
    }

    /* Set and get projected height */
    inline __attribute__ ((always_inline)) static int zterminal_set_projected_height(zterminal* obj, double height)
    {
	ZCHECK_OBJ_INT(obj);
	obj->term_height = height;
	return 0;
    }
    inline __attribute__ ((always_inline)) static double zterminal_get_projected_Height(zterminal* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->term_height;
    }

#ifdef __cplusplus
}
#endif
#endif /* _ZTERMINAL_H_ */
