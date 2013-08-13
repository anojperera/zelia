/* Header file for declaring interfaces for Cell object of
 * table. Inherites the base class and this class shall
 * derive column and row collections.
 * Sat Mar  3 13:00:22 GMT 2012 */

#ifndef _ZTCELL_H_
#define _ZTCELL_H_

#include <stdlib.h>
#include "zBase.h"

/* Content buffer size */
#define Z_TCELL_BUFF 256

/* Forward declaration of struct */
typedef struct _zTCell zTCell;

struct _zTCell
{
    zBase z_parent;			/* Inherited parent object */
    unsigned int z_int_flg;		/* Internal flag */
    char z_content[Z_TCELL_BUFF];	/* Content buffer */
    unsigned int _z_row_ix;		/* Internal row index */
    unsigned int _z_col_ix;		/* Internal column index */
    unsigned int z_line_flg;		/* Line flag */
    void* z_child;			/* Child pointer */
    zgeneric_fptr z_draw_func;		/* Draw function pointer */
    size_t z_obj_sz;			/* Object size */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGeneric* zTCell_New(zTCell* obj);
    void zTCell_Delete(zTCell* obj);

    /* Draw function */
    int zTCell_Draw(zTCell* obj);

    /* Properties methods */

    /* Set and get cell content */
    inline __attribute__ ((always_inline)) static int zTCell_Set_Content(zTCell* obj, const char* content)
    {
	Z_CHECK_OBJ(obj);
	Z_CHECK_OBJ(content);
	strcpy(obj->z_content, content);
	return 0;
    }
    inline __attribute__ ((always_inline)) static const char* zTCell_Get_Content(zTCell* obj)
    {
	Z_CHECK_OBJ_PTR(obj);
	return obj->z_content;
    }

    /* Get row and column index */
    inline __attribute__ ((always_inline)) static unsigned int zTCell_Get_Row_Ix(zTCell* obj)
    {
	Z_CHECK_OBJ(obj);
	return obj->_z_row_ix;
    }
    inline __attribute__ ((always_inline)) static unsigned int zTCell_Get_Col_Ix(zTCell* obj)
    {
	Z_CHECK_OBJ(obj);
	return obj->_z_col_ix;
    }

    /* Set and get line flag */
    inline __attribute__ ((always_inline)) static int zTCell_Set_Line_Flg(zTCell* obj, unsigned int flg)
    {
	Z_CHECK_OBJ(obj);
	obj->z_line_flg = flg;
	return 0;
    }
    inline __attribute__ ((always_inline)) static unsigned int zTCell_Get_Line_Flg(zTCell* obj)
    {
	if(obj == NULL)
	    return 0;
	else
	    return obj->z_line_flg;
    }

#ifdef __cplusplus
}
#endif

#endif /* _ZTCELL_H_ */
