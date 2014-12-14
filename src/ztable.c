/* Implementation of table class */
/* Tue Mar  6 12:44:15 GMTST 2012 */

#include "ztable.h"
#include "zVar.h"
/* Virtual function */

static int _ztable_draw(void* obj);
static int _ztable_delete(void* obj);

/* Constructor */
zgeneric* ztable_new(ztable* obj)
{
    ZCONSTRUCTOR(obj, ztable);

    /* create parent object */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);

	    return NULL;
	}

    /* Set properties */
    obj->num_rows = 0;
    obj->num_cols = 0;
    obj->arr_flg = 0;
    obj->col_widths = NULL;
    obj->child = NULL;

    /* initialise vtable */
    ZGENERIC_INIT_VTABLE(obj);

    /* set drawing pointer */
    zgeneric_set_draw(obj, _ztable_draw);
    zgeneric_set_delete_callback(obj, _ztable_delete);

    /* set child pointer */
    zgeneric_set_child_pointer(obj);

    ZELIA_LOG_MESSAGE("ztable object created");
    /* return top level parent object */
    return obj->super_cls;
}

/* Destructor */
void ztable_delete(ztable* obj)
{
    /* Check object */
    ZCHECK_OBJ_VOID(obj);

    if(obj->vtable.zgeneric_delete)
	{
	    obj->vtable.zgeneric_delete((void*) obj->super_cls);
	    return;
	}

    /* delete parent */
    zgeneric_block_parent_destructor(obj);
    zbase_delete(&obj->parent);


    /* call to delete row collection */
    if(obj->arr_flg)
	ztrows_delete(&obj->rows);

    if(obj->col_widths)
	free(obj->col_widths);

    obj->child = NULL;
    obj->super_cls = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    /* if object was internally created
     * delete it */
    if(ZDESTRUCTOR_CHECK)
	free(obj);

    ZELIA_LOG_MESSAGE("ztable object deleted");
    return;
}

/* Draw method */
int ztable_draw(ztable* obj)
{
    
    /* check object */
    ZCHECK_OBJ_INT(obj);

    /* check if array was created */
    if(obj->arr_flg == 0)
	return ZELIA_TABLE_ERROR;

    /* call draw function of row collection */
    return zgenerics_draw(obj->rows.super_cls);
}


/*=================================== Property Methods ===================================*/
/* Set rows and columns */
int ztable_set_rows_and_cols(ztable* obj,
			     unsigned int num_rows,
			     unsigned int num_cols)
{
    int _i;
    zdevice* _dev;

    ZCHECK_OBJ_INT(obj);

    obj->num_rows = num_rows;
    obj->num_cols = num_cols;

    /* Check if array was created */
    if(obj->arr_flg == 0)
	{
	    /* create an array of pointers to hold column width and
	     * initialise to 0.0 */
	    obj->col_widths = (double*) calloc(num_cols, sizeof(double));
	    for(_i=0; _i<num_cols; _i++)
		obj->col_widths[_i] = 0.0;

	    _dev = zgeneric_get_device(obj->super_cls);

	    ztrows_new(&obj->rows,						/* row object */
		       _dev,							/* device object */
		       obj->num_rows,						/* number of rows */
		       obj->num_cols,						/* number of columns */
		       obj->parent.x,						/* origin x */
		       obj->parent.y,						/* origin y */
		       zbase_get_width(&obj->parent),				/* row width */
		       zbase_get_height(&obj->parent) / (double) num_rows);	/* row height */
	    obj->arr_flg = 1;
	}

    return ZELIA_OK;
}


/* Set content */
int ztable_set_content(ztable* obj,
		       unsigned int row_ix,
		       unsigned int col_ix,
		       const char* content)
{
    ztrow* _trow;

    /* object check */
    ZCHECK_OBJ_INT(obj);

    /* check collection bounds */
    if(row_ix >= obj->num_rows ||
       col_ix >= obj->num_cols ||
       obj->arr_flg == 0)
	return 1;

    /* get row specified by the index */
    _trow = ztrows_get_row(&obj->rows, row_ix);

    /* check object */
    ZCHECK_OBJ_INT(_trow);

    return ztrow_add_content(_trow, col_ix, content);
}

/* Get content */
const char* ztable_get_content(ztable* obj,
			       unsigned int row_ix,
			       unsigned int col_ix)
{
    ztcell* _tcell;
    ztrow* _trow;

    /* check object */
    ZCHECK_OBJ_PTR(obj);

    /* check collection bounds */
    if(row_ix > obj->num_rows || col_ix > obj->num_cols)
	return NULL;

    /* get row specified by the index */
    _trow = ztrows_get_row(&obj->rows, row_ix);
    ZCHECK_OBJ_PTR(_trow);

    _tcell = ztrow_get_cell(_trow, col_ix);
    ZCHECK_OBJ_PTR(_tcell);

    return ztcell_get_content(_tcell);
}


/* Get cell specified by row index and column index */
const ztcell* ztable_get_cell(ztable* obj,
			      unsigned int row_ix,
			      unsigned int col_ix)
{
    ztrow* _trow;

    /* object check */
    ZCHECK_OBJ_PTR(obj);

    if(row_ix > obj->num_rows || col_ix > obj->num_cols)
	return NULL;

    _trow = ztrows_get_row(&obj->rows, row_ix);
    ZCHECK_OBJ_PTR(_trow);

    return ztrow_get_cell(_trow, col_ix);
}

/* Set column width */
int ztable_set_column_width(ztable* obj,
			    unsigned int col_ix,
			    double width)
{
    ztrow* _trow;
    ztcell* _tcell;
    int _i, _a;

    /* Check for object */
    ZCHECK_OBJ_INT(obj);

    /* check if column width array was created */
    ZCHECK_OBJ_INT(obj->col_widths);

    /* check array bounds */
    if(col_ix >= obj->num_cols)
	return ZELIA_TABLE_ERROR;

    obj->col_widths[col_ix] = width;

    /* check if array created */
    if(obj->arr_flg == 0)
	return ZELIA_TABLE_ERROR;

    /* iterate through rows collection and set the width */
    for(_i=0; _i<obj->num_rows; _i++)
	{
	    _trow = ztrows_get_row(&obj->rows, (unsigned int) _i);
	    _tcell = ztrow_get_cell(_trow, col_ix);

	    /* set width */
	    zbase_set_width(&_tcell->parent, width);
	    for(_a=col_ix+1; _a<obj->num_cols; _a++)
		{
		    _tcell = ztrow_get_cell(_trow, _a);
		    if(_tcell == NULL)
			break;
		    _tcell->parent.x += width - _tcell->parent.width;
		}
	}

    return ZELIA_OK;
}


/*=================================== Private Methods ===================================*/

/* Virtual draw method */
static int _ztable_draw(void* obj)
{
    int _rt = ZELIA_OK;
    zgeneric* _zg;
    ztable* _self;
    
    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;
    _self = Z_TABLE(_zg);
    
    _rt = ztable_draw(_self);

    /* if the callback pointer of child class set, call it */
    if(_self->vtable.zgeneric_draw)
	return _self->vtable.zgeneric_draw(obj);

    return _rt;
}

static int _ztable_delete(void* obj)
{
    zgeneric* _zg = NULL;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    ztable_delete(Z_TABLE(_zg));
    return ZELIA_OK;
}
