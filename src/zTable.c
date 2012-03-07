/* Implementation of table class */
/* Tue Mar  6 12:44:15 GMTST 2012 */

#include "zTable.h"

/* Virtual function */
static int _ztable_draw(zGeneric* obj);

/* Constructor */
zGeneric* zTable_New(zTable* obj)
{
    if(obj == NULL)
	{
	    obj = (zTable*) malloc(sizeof(zTable));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

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
    
    zDevice* _dev;
    Z_CHECK_OBJ(obj);
    obj->z_num_rows = num_rows;
    obj->z_num_cols = num_cols;

    /* Check if array was created */
    if(obj->z_arr_flg == 0)
	{
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
    zTCell* _tcell;
    zTRow* _trow;
    /* object check */
    Z_CHECK_OBJ(obj);

    /* check collection bounds */
    if(row_ix > obj->z_num_rows ||
       col_ix > obj->z_num_cols ||
       obj->z_arr_flg == 0)
	return 1;

    /* get row specified by the index */
    _trow = zTRows_Get_Row(&obj->z_rows, row_ix);
    Z_CHECK_OBJ(_trow);

    _tcell = zTRow_Get_Cell(_trow, col_ix);
    Z_CHECK_OBJ(_tcell);

    /* Set content */
    zTCell_Set_Content(_tcell, content);
    return 0;
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

/* Private functions */
/*************************************************************************************************************/

/* Virtual draw method */
static int _ztable_draw(zGeneric* obj)
{
    Z_CHECK_OBJ(obj);
    return zTable_Draw(Z_TABLE(obj));
}
