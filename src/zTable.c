/* Implementation of table class */
/* Tue Mar  6 12:44:15 GMTST 2012 */

#include "zTable.h"
#include "zVar.h"
/* Virtual function */
static int _ztable_draw(zGeneric* obj);

/* Constructor */
zGeneric* zTable_New(zTable* obj)
{
    Z_CONSTRUCTOR_HELPER(obj, zTable);
    /* create parent object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->z_int_flg = 0;
    obj->z_num_rows = 0;
    obj->z_num_cols = 0;
    obj->z_col_widths = NULL;
    obj->z_child = NULL;
    obj->z_draw_func = NULL;
    obj->z_obj_sz = sizeof(zTable);

    /* set function pointers of parent */
    obj->z_parent.z_draw_func = _ztable_draw;

    /* child pointer of parent object */
    obj->z_parent.z_child = (void*) obj;

    /* return top level parent object */
    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zTable_Delete(zTable* obj)
{
    /* Check object */
    Z_CHECK_OBJ_VOID(obj);

    /* delete parent */
    zBase_Delete(&obj->z_parent);

    /* call to delete row collection */
    if(obj->z_arr_flg)
	zTRows_Delete(&obj->z_rows);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* if object was internally created
     * delete it */
    if(obj->z_int_flg)
	free(obj);
}

/* Draw method */
int zTable_Draw(zTable* obj)
{
    /* check object */
    Z_CHECK_OBJ(obj);

    /* check if array was created */
    if(obj->z_arr_flg == 0)
	return 1;

    /* call draw function of row collection */
    return zGenerics_Draw(&obj->z_rows.z_parent);
}


/* Property methods */
/**************************************************************************************************************/

/* Set rows and columns */
inline int zTable_Set_Rows_and_Cols(zTable* obj,
				    unsigned int num_rows,
				    unsigned int num_cols)
{
    int i;
    zDevice* _dev;
    Z_CHECK_OBJ(obj);
    obj->z_num_rows = num_rows;
    obj->z_num_cols = num_cols;

    /* Check if array was created */
    if(obj->z_arr_flg == 0)
	{
	    /* create an array of pointers to hold column width and
	     * initialise to 0.0 */
	    obj->z_col_widths = (double*) calloc(num_cols, sizeof(double));
	    for(i = 0; i < num_cols; i++)
		obj->z_col_widths[i] = 0.0;
	    
	    _dev = zGeneric_Get_Device(&obj->z_parent.z_sgeneric);
	    
	    zTRows_New(&obj->z_rows,						/* row object */
		       _dev,							/* device object */
		       obj->z_num_rows,						/* number of rows */
		       obj->z_num_cols,						/* number of columns */
		       obj->z_parent.z_x,					/* origin x */
		       obj->z_parent.z_y,					/* origin y */
		       zBase_Get_Width(&obj->z_parent),				/* row width */
		       zBase_Get_Height(&obj->z_parent) / (double) num_rows);	/* row height */
	    obj->z_arr_flg = 1;
	}
    return 0;
}

/* Get number of rows */
inline unsigned int zTable_Get_Rows(zTable* obj)
{
    if(obj == NULL)
	return 0;
    else
	return obj->z_num_rows;
}

/* Get number of columns */
inline unsigned int zTable_Get_Cols(zTable* obj)
{
    if(obj == NULL)
	return 0;
    else
	return obj->z_num_cols;
}

/* Set content */
inline int zTable_Set_Content(zTable* obj,
			      unsigned int row_ix,
			      unsigned int col_ix,
			      const char* content)
{
    zTRow* _trow;
    /* object check */
    Z_CHECK_OBJ(obj);

    /* check collection bounds */
    if(row_ix >= obj->z_num_rows ||
       col_ix >= obj->z_num_cols ||
       obj->z_arr_flg == 0)
	return 1;

    /* get row specified by the index */
    _trow = zTRows_Get_Row(&obj->z_rows, row_ix);
    Z_CHECK_OBJ(_trow);
    return zTRow_Add_Content(_trow, col_ix, content);
}

/* Get content */
inline const char* zTable_Get_Content(zTable* obj,
				      unsigned int row_ix,
				      unsigned int col_ix)
{
    zTCell* _tcell;
    zTRow* _trow;

    /* check object */
    Z_CHECK_OBJ_PTR(obj);

    /* check collection bounds */
    if(row_ix > obj->z_num_rows ||
       col_ix > obj->z_num_cols)
	return NULL;

    /* get row specified by the index */
    _trow = zTRows_Get_Row(&obj->z_rows, row_ix);
    Z_CHECK_OBJ_PTR(_trow);

    _tcell = zTRow_Get_Cell(_trow, col_ix);
    Z_CHECK_OBJ_PTR(_tcell);

    return zTCell_Get_Content(_tcell);
}


/* Get row specified by the row index */
inline const zTRow* zTable_Get_Row(zTable* obj,
				       unsigned int row_ix)
{
    /* check row object */
    Z_CHECK_OBJ_PTR(obj);

    /* check number of rows */
    if(row_ix > obj->z_num_rows)
	return NULL;

    return zTRows_Get_Row(&obj->z_rows, row_ix);
}

/* Get cell specified by row index and column index */
inline const zTCell* zTable_Get_Cell(zTable* obj,
					 unsigned int row_ix,
					 unsigned int col_ix)
{
    zTRow* _trow;

    /* object check */
    Z_CHECK_OBJ_PTR(obj);
    
    if(row_ix > obj->z_num_rows ||
       col_ix > obj->z_num_cols)
	return NULL;
    _trow = zTRows_Get_Row(&obj->z_rows, row_ix);
    Z_CHECK_OBJ_PTR(_trow);
    return zTRow_Get_Cell(_trow, col_ix);
}

/* Set column width */
inline int zTable_Set_Column_Width(zTable* obj,
				       unsigned int col_ix,
				       double width)
{   zTRow* _trow;
    zTCell* _tcell;
    int i, a;
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* check if column width array was created */
    Z_CHECK_OBJ(obj->z_col_widths);

    /* check array bounds */
    if(col_ix >= obj->z_num_cols)
	return 1;

    obj->z_col_widths[col_ix] = width;

    /* check if array created */
    if(obj->z_arr_flg == 0)
	return 1;

    /* iterate through rows collection and set the width */
    for(i=0; i<obj->z_num_rows; i++)
	{
	    _trow = zTRows_Get_Row(&obj->z_rows, (unsigned int) i);
	    _tcell = zTRow_Get_Cell(_trow, col_ix);

	    /* set width */
	    zBase_Set_Width(&_tcell->z_parent, width);
	    for(a=col_ix+1; a<obj->z_num_cols; a++)
		{
		    _tcell = zTRow_Get_Cell(_trow, a);
		    if(_tcell == NULL)
			break;
		    _tcell->z_parent.z_x += width - _tcell->z_parent.z_width;
		}
	}

    return 0;
}

/* Get column width */
inline double zTable_Get_Column_Width(zTable* obj,
				      unsigned int col_ix)
{
    /* check object */
    Z_CHECK_OBJ_DOUBLE(obj);
    
    /* check if column width array was created */
    Z_CHECK_OBJ_DOUBLE(obj->z_col_widths);

    /* check array bounds */
    if(col_ix >= obj->z_num_cols)
	return 0.0;

    return obj->z_col_widths[col_ix];
    
}
/* Private functions */
/*************************************************************************************************************/

/* Virtual draw method */
static int _ztable_draw(zGeneric* obj)
{
    Z_CHECK_OBJ(obj);
    return zTable_Draw(Z_TABLE(obj));
}
