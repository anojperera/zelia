/* Generic collection object
 * Sun Jan 22 23:30:38 GMT 2012 */

#ifndef _ZGENERICS_H_
#define _ZGENERICS_H_

#include <stdlib.h>
#include "../../g_list/src/alist.h"
#include "zGeneric.h"
#include "zDevice.h"

/* Forward declaration of object */
typedef struct _zGenerics zGenerics;

/* Function pointer for drawing functions in collection */
typedef int (*zcollection_fptr)(zGeneric*, void*);


struct _zGenerics
{
    aNode* z_generics_d;				/* dynamically expandable collection */
    zGeneric** z_generics_s;				/* static collection */
    unsigned int z_count;				/* count of objects */
    unsigned int z_expansion_flg;			/* expansion flag */
    unsigned int z_int_flg;				/* internal flag */
    zcollection_fptr z_draw_func;			/* Draw function pointer */
    zcollection_fptr z_destructor_func;			/* Function pointer for child destructor */
    zDevice* z_device;					/* Device object */
    void* z_usr_data;					/* User data */
    void* z_child;					/* child object */
    size_t z_obj_sz;					/* object size */
};

/* Following C functions are only to be called internally within the library */

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    /* If static option was used, object count must be greater than
     * 1. The contructor shall fail if object count is less than 1
     * and static flag was specified */
    int zGenerics_New(zGenerics* obj,
		      unsigned int s_flg,		/* static or dynamic option */
		      unsigned int g_count);		/* count of objects if static option was used */

    void zGenerics_Delete(zGenerics* obj);

    /* Get object count */
    inline unsigned int zGenerics_Get_Count(zGenerics* obj);

    /* Set and get device object */
    inline int zGenerics_Set_Device(zGenerics* obj, zDevice* dev);
    inline zDevice* zGenerics_Get_Device(zGenerics* obj);

    /* Clear array */
    void zGenerics_Clear(zGenerics* obj);

    /* Draw function */
    int zGenerics_Draw(zGenerics* obj);
#ifdef __cplusplus
}
#endif

#endif /* _ZGENERICS_H_ */
