/* Generic collection object
 * Sun Jan 22 23:30:38 GMT 2012 */

#ifndef _ZGENERICS_H_
#define _ZGENERICS_H_

#include <stdlib.h>
#include <blist.h>
#include "zgeneric.h"
#include "zdevice.h"

/* Forward declaration of object */
typedef struct _zgenerics zgenerics;


struct _zgenerics
{
    unsigned int _init_flg;				/* internal flag */
    unsigned int count;					/* count of objects */
    unsigned int expansion_flg;				/* expansion flag */


    blist generics_d;					/* dynamically expandable collection */
    zgeneric** generics_s;				/* static collection */

    zdevice* device;					/* Device object */
    struct _zgeneric_vtable vtable;			/* vtable for delete, draw and new function pointers */
    void* usr_data;					/* User data */
    void* child;					/* child object */
};

/* Following C functions are only to be called internally within the library */
#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    /* If static option was used, object count must be greater than
     * 1. The contructor shall fail if object count is less than 1
     * and static flag was specified */
    int zgenerics_new(zgenerics* obj,
		      unsigned int s_flg,			/* static or dynamic option */
		      unsigned int g_count);			/* count of objects if static option was used */

    void zgenerics_delete(zgenerics* obj);

    /* Get object count */
    inline __attribute__ ((always_inline)) static unsigned int zgenerics_get_count(zgenerics* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->count;
    }

    /* Set and get device object */
    inline __attribute__ ((always_inline)) static int zgenerics_set_device(zgenerics* obj, zdevice* dev)
    {
	ZCHECK_OBJ_INT(obj);
	ZCHECK_OBJ_INT(dev);

	obj->device = dev;
	return 0;
    }

    /* Get device */
    inline __attribute__ ((always_inline)) static zdevice* zgenerics_get_device(zgenerics* obj)
    {
	ZCHECK_OBJ_PTR(obj);
	return obj->device;
    }

    /* Clear array */
    void zgenerics_clear(zgenerics* obj);

    /* Draw function */
    int zgenerics_draw(zgenerics* obj);
#ifdef __cplusplus
}
#endif

#endif /* _ZGENERICS_H_ */
