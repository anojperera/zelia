/* Header file for column collection.
 * A column collection shall be associated with all rows in a table
 * Sat Feb  4 22:12:33 GMT 2012 */

#ifndef _ZTCOLUMNS_H_
#define _ZTCOLUMNS_H_

#include "zGenerics.h"
#include "zDevice.h"
#include <stdlib.h>

/* Forward declaration of struct */
typedef struct _zTColumns zTColumns;

struct _zTColumns
{
    zGenerics z_parent;				/* inherited parent object */
    unsigned int z_int_flg;			/* internal flag */
    void* z_child;				/* child object */
    zcollection_fptr z_draw_func;		/* draw function pointer */
    size_t z_obj_sz;				/* object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGenerics* zTColumns_New(zTColumns* obj,			/* optional object */
			     zDevice* dev,			/* device object */
			     unsigned int num_col,		/* number of columns */
			     double width);			/* column widht */
    
    void zTColumns_Delete(zTColumns* obj);


    /* Use collection draw method */
    /* int zTColumns_Draw(zTColumns* obj); */
    

#ifdef __cplusplus
}
#endif

#endif /* _ZTCOLUMNS_H_ */
