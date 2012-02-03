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

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGenerics* zGlands_New(void);
    void zGlands_Delete(zGenerics* obj);

    /* Add gland to collection */
    int zGlands_Add(zGenerics* obj,
		      zDevice* dev,
		      double x,
		      double y,
		      zGlandSize sz,
		      unsigned int hex_flg);

    
#ifdef __cplusplus
}
#endif

#endif /* _ZGLANDS_H_ */
