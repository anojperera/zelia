/* Implementation of zTRow class */
/* Sun Mar  4 18:00:47 GMT 2012 */

#include "zTRow.h"

/* Virtual function */
static int _ztrow_draw(zGeneric* obj);

/* Constructor */
zGeneric* zTRow_New(zTRow* obj, unsigned int ix)
{
    /* Check for object */
    if(obj == NULL)
	{
	    obj = (zTRow*) malloc(sizeof(zTRow));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create base object */
    if(!zBase_New(&obj->z_parent))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->z_arr_flg = 0;		/* Flag to indicate tcells array not created */
    obj->z_num_cols = 0;	/* number of columns */
    obj->z_child = NULL;
    obj->z_draw_func = NULL;
    obj->z_obj_sz = sizeof(zTRow);
    obj->z_ix = ix;
    
    /* Set function and child pointer of parent
     * object */
    obj->z_parent.z_draw_func = _ztrow_draw;

    obj->z_parent.z_child = (void*) obj;

    /* Return top level object */
    return &obj->z_parent.z_sgeneric;
}

/* Destructor */
void zTRow_Delete(zTRow* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* if tcells was created destroy */
    if(obj->z_arr_flg)
	zTCells_Delete(&obj->z_tcells);

    /* delete parent object */
    zBase_Delete(&obj->z_parent);

    if(obj->z_int_flg)
	free(obj);
}

/* Draw function */
int zTRow_Draw(zTRow* obj)
{
    zDevice* _dev;
    zGeneric* _zg;
    zGenerics* _zgs;
    zBase* _base;
    
    /* check for object */
    Z_CHECK_OBJ(obj);

    /* if number of columns were not set exit */
    if(obj->z_num_cols == 0)
	return 1;
    
    _zg = &obj->z_parent.z_sgeneric;
    _base = &obj->z_parent;
    _dev = zGeneric_Get_Device(_zg);

    /* check if tcell collection was created or not */
    if(obj->z_arr_flg == 0)
	{
	    zTCells_New(&obj->z_tcells,
			_dev,
			obj->z_ix,
			obj->z_num_cols,
			_base->z_x,
			_base->z_y,
			_base->z_width / (double) obj->z_num_cols,
			_base->z_height);
	    obj->z_arr_flg = 1;
	}
    _zgs = &obj->z_tcells.z_parent;
    
    return zGenerics_Draw(_zgs);
}

/* Add content to the cell */
inline int zTRow_Add_Content(zTRow* obj,
			     unsigned int ix,		/* Column index */
			     const char* content)	/* Content */
{
    zGenerics* _zg;
    zDevice* _dev;
    /* check object */
    Z_CHECK_OBJ(obj);

    /* check index is over bound */
    if(ix > obj->z_num_cols)
	return 1;

    /* check if tcell collection was created or not */
    if(obj->z_arr_flg == 0)
	{
	    _dev = zGeneric_Get_Device(&obj->z_parent.z_sgeneric);
	    zTCells_New(&obj->z_tcells,
			_dev,
			obj->z_ix,
			obj->z_num_cols,
			obj->z_parent.z_x,
			obj->z_parent.z_y,
			obj->z_parent.z_width / (double) obj->z_num_cols,
			obj->z_parent.z_height);
	    obj->z_arr_flg = 1;
	}

    _zg = &obj->z_tcells.z_parent;
    return zTCell_Set_Content(Z_TCELL(_zg->z_generics_s[ix]),
			      content);
}

/* Property methods */
/*********************************************************************/

/* Get Cell */
inline zTCell* zTRow_Get_Cell(zTRow* obj,
			      unsigned int ix)
{
    zGenerics* _zg;
    
    Z_CHECK_OBJ_PTR(obj);

    /* check array bounds */
    if(ix > obj->z_num_cols || obj->z_arr_flg == 0)
	return NULL;

    _zg = &obj->z_tcells.z_parent;
    return Z_TCELL(_zg->z_generics_s[ix]);
}

/* Set number of columns */
inline int zTRow_Set_Num_Cols(zTRow* obj, unsigned int num_cols)
{
    Z_CHECK_OBJ(obj);
    obj->z_num_cols = num_cols;
    return 0;
}

/* Get number of columns */
inline unsigned int zTRow_Get_Num_Cols(zTRow* obj)
{
    if(obj == NULL)
	return 0;
    else
	return obj->z_num_cols;
}

/* Private methods */
/**********************************************************************/

/* Virtual draw method */
static int _ztrow_draw(zGeneric* obj)
{
    Z_CHECK_OBJ(obj);
    return zTRow_Draw(Z_TROW(obj));
}
