/* Header file for glands interface.
 * Cable glands collection is created as a
 * dynamically expanding zGenerics collection. zGlands_Add function
 * should be use when adding new glands to the collection.
 * Fri Feb  3 17:33:18 GMT 2012 */

#ifndef _ZGLANDS_H_
#define _ZGLANDS_H_

#include <stdlib.h>
#include "zDevice.h"
#include "zGenerics.h"

typedef struct _zGlands zGlands;

struct _zGlands
{
    zGenerics z_parent;
    unsigned int z_int_flg;
    void* z_child;					/* Child object */
    zcollection_fptr z_draw_func;			/* Draw function pointer */
    size_t z_obj_sz;					/* size of object */
};


#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGenerics* zGlands_New(zGlands* obj);
    void zGlands_Delete(zGlands* obj);

    /* Add gland to collection */
    int zGlands_Add(zGlands* obj,
		    zDevice* dev,
		    double x,
		    double y,
		    zGlandSize sz,
		    unsigned int hex_flg);

    
#ifdef __cplusplus
}
#endif

#endif /* _ZGLANDS_H_ */
