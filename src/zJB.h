/* Header file for declaraing interfaces for junction boxes.
 * inherits the base class.
 * JB classes can be associated with a collection of cable glands
 * and terminals
 * Mon Jan 30 23:05:29 GMT 2012 */

#ifndef _ZJB_H_
#define _ZJB_H_

#include "zBase.h"
#include "zGeneric.h"
#include "zGenerics.h"
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
    double z_ang;				/* Orientation angle */
    zGenerics* z_terms;				/* Terminals collection */
    zGenerics* z_glands;			/* cable gland collection */
    size_t z_obj_sz;				/* object size */
    void* z_child;				/* child pointer */

    zgeneric_fptr z_draw_func;			/* draw function pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zGeneric* zJB_New(zJB* obj, double width, double height, double depth, double ang);
    void zJB_Delete(zJB* obj);

    /* Draw function */
    int zJB_Draw(zJB* obj);

    /* Property methods */
    /***********************************************************************/
    
    /* Set and get terminals collection */
    inline int zJB_Set_Terminals(zJB* obj, zGenerics* terms);
    inline zGenerics* zJB_Get_Terminals(zJB* obj);

    /* Add terminal collection */
    inline int zJB_Add_Terminals(zJB* obj,
				 unsigned int num_term,			/* number of terminals */
				 double width,				/* terminal width */
				 double height,				/* terminal height */
				 const char* links);			/* terminal links */
    /* Set and get gland collection */
    inline int zJB_Set_Glands(zJB* obj, zGenerics* glands);
    inline zGenerics* zJB_Get_Glands(zJB* obj);
    
#ifdef __cplusplus
}
#endif

#endif /* _ZJB_H_ */
