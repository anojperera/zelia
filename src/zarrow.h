/* Generic arrow head class, to be inherited by all
 * Mon Feb  6 21:57:45 GMT 2012 */

#ifndef _ZARROW_H_
#define _ZARROW_H_

#include <stdlib.h>

#include "zgeneric.h"
#include "zbase.h"
#include "zVar.h"

/* Forward declaration of struct */
typedef struct _zarrow zarrow;

struct _zarrow
{
    zbase parent;				/* inherited parent object */
    unsigned int _init_flg;			/* internal flag */
    zArrowTypes arrow_type;			/* arrow head type */
    unsigned int fill_flg;			/* flag to indicate fill arrow */

    zgeneric* super_cls;			/* super class pointer */
    void* child;				/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zgeneric* zarrow_new(zarrow* obj);
    void zarrow_delete(zarrow* obj);

    /* Draw function */
    int zarrow_draw(zarrow* obj);

    /*************************************************************/
    /* Property methods */

    /* Set and get fill flag */
    inline __attribute__ ((always_inline)) static int zarrow_set_fill_flag(zarrow* obj, unsigned int flg)
    {
	ZCHECK_OBJ_INT(obj);
	obj->fill_flg = flg;
	return ZELIA_OK;
    }

    inline __attribute__ ((always_inline)) static unsigned int zarrow_get_fill_flag(zarrow* obj)
    {
	ZCHECK_OBJ_INT(obj);
	return obj->fill_flg;
    }

    /* Set and get arrow head type */
    inline __attribute__ ((always_inline)) static int zarrow_set_arrow_type(zarrow* obj, zArrowTypes type)
    {
	ZCHECK_OBJ_INT(obj);
	obj->arrow_type = type;
	return ZELIA_OK;
    }

    inline __attribute__ ((always_inline)) static zArrowTypes zarrow_get_arrow_type(zarrow* obj)
    {
	if(obj == NULL)
	    return zArrow1;
	else
	    return obj->arrow_type;
    }

#ifdef __cplusplus
}
#endif

#endif /* _ZARROW_H_ */
