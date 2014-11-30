/* Header file for glands interface.
 * Cable glands collection is created as a
 * dynamically expanding zGenerics collection. zGlands_Add function
 * should be use when adding new glands to the collection.
 * Fri Feb  3 17:33:18 GMT 2012 */

#ifndef _ZGLANDS_H_
#define _ZGLANDS_H_

#include <stdlib.h>
#include "zdevice.h"
#include "zgenerics.h"

typedef struct _zglands zglands;

struct _zglands
{
    zgenerics parent;
    unsigned int _int_flg;

    struct _zgeneric_vtable vtable;			/* vtable */
    zgenerics* super_cls;				/* super class pointer */
    void* child;					/* Child object */
};


#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgenerics* zglands_new(zglands* obj);
    void zglands_delete(zglands* obj);

    /* Add gland to collection */
    int zglands_add(zglands* obj,
		    zdevice* dev,
		    double x,
		    double y,
		    zGlandSize sz,
		    unsigned int hex_flg);

    
#ifdef __cplusplus
}
#endif

#endif /* _ZGLANDS_H_ */
