/* Header file for declaraing interfaces for junction boxes.
 * inherits the base class.
 * JB classes can be associated with a collection of cable glands
 * and terminals */
 * Mon Jan 30 23:05:29 GMT 2012 */

#ifndef _ZJB_H_
#define _ZJB_H_

#include "zBase.h"
#include <stdlib.h>


/* Forward declaration of struct */
typedef struct _zJB zJB;

struct _zJB
{
    zBase z_parent;				/* Inherited parent object */
    unsigned int z_int_flg;			/* Internal flag */
    unsigned int z_terms_ext;			/* Flag to indicate terminal collection
						 * externally set */
    unsigned int z_glands_ext;			/* Flag to indicate glads collection external */
    double z_depth;				/* Depth of junction box */
    zGenerics* z_terms;				/* Terminals collection */
    zGenerics* z_glands;			/* cable gland collection */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zGeneric* zJB_New(zJB* obj);
    void zJB_Delete(zJB* obj);

    /* Draw function */
    int zJB_Draw(zJB* obj);

    /* Property methods */
    /***********************************************************************/

    /* Set and get terminals collection */
    
    
#endif __cplusplus
}
#endif

#endif /* _ZJB_H_ */
