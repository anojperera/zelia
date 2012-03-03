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
    double _z_cell_width;		/* Internal column width */
    double _z_cell_height;		/* Internal column height */
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
    inline int zTCell_Set_Content(zTCell* obj, const char* content);
    inline const char* zTCell_Get_Content(zTCell* obj);

    /* Get row and column index */
    inline unsigned int zTCell_Get_Row_Ix(zTCell* obj);
    inline unsigned int zTCell_Get_Col_Ix(zTCell* obj);

    /* Get cell width and height */
    inline double zTCell_Get_Width(zTCell* obj);
    inline double zTCell_Get_Height(zTCell* obj);

    /* Set and get line flag */
    inline int zTCell_Set_Line_Flg(zTCell* obj, unsigned int flg);
    inline unsigned int zTCell_Get_Line_Flg(zTCell* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZTCELL_H_ */
