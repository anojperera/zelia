/* Implementation of zTCells class */
/* Sun Mar  4 12:57:34 GMT 2012 */

#include "zTCells.h"
#include "zVar.h"

/* Virtual functions */
static int _ztrow_delete(void* obj, void* usr_data);		/* delete function */
static int _ztrow_draw(void* obj, void* user_data);		/* draw function */

/* Constructor */
zGenerics* zTCells_New(zTCells* obj,		/* Optional argument */
		     zDevice* dev,		/* Device object */
		     unsigned int row_ix,	/* row index */
		     unsigned int num_col,	/* Number of columns */
		     double x,			/* origin x */
		     double y,			/* origin y */
		     double col_width,		/* Column width */
		     double row_height)		/* Row height */
{
    int i;
    zTCell* _tcell;
    /* check for object */
    if(obj == NULL)
	{
	    obj = (zTCells*) malloc(sizeof(zTCells));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create parent object */
    if(zGenerics_New(&obj->z_parent,
		     0,
		     num_col))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* Set device */
    zGenerics_Set_Device(&obj->z_parent, dev);

    /* Set properties */
    obj->z_width = col_width;
    obj->z_height = row_height;
    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    /* Create object array */
    for(i=0; i<num_col; i++)
	{
	    obj->z_parent.z_generics_s[i] =
		zTCell_New(NULL);

	    _tcell = Z_TCELL(obj->z_parent.z_generics_s[i]);

	    /* Set index, width and height */
	    _tcell->_z_row_ix = row_ix;
	    _tcell->_z_col_ix = (unsigned int) i;

	    /* Set base coordinates */
	    zBase_Set_Base_Coords(Z_BASE(obj->z_parent.z_generics_s[i]),
				  x + (double) i * col_width,
				  y);
	    
	    /* Set width and height of cell */
	    zBase_Set_Width_and_Height(Z_BASE(obj->z_parent.z_generics_s[i]),
				       col_width,
				       row_height);

	    /* Set default device context */
	    zGeneric_Set_Device(obj->z_parent.z_generics_s[i],
				zGenerics_Get_Device(&obj->z_parent));
	    
	    /* Set default device context */
	    zGeneric_Set_Default_Dev_Context(obj->z_parent.z_generics_s[i]);
	    
	    /* Set line flag */
	    zTCell_Set_Line_Flg(_tcell, 1);
	}

    /* Set parent properties */
    obj->z_parent.z_destructor_func = _ztrow_delete;
    obj->z_parent.z_draw_func = _ztrow_draw;

    /* Parent user data set as self */
    obj->z_parent.z_usr_data = (void*) obj;

    /* Set child poitner of parent */
    obj->z_parent.z_child = (void*) obj;

    obj->z_obj_sz = sizeof(zTCells);
    
    return &obj->z_parent;
}

/* Destructor */
void zTCells_Delete(zTCells* obj)
{
    /* check object */
    Z_CHECK_OBJ_VOID(obj);

    /* call delete method of parent object */
    zGenerics_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    if(obj->z_int_flg)
	free(obj);
}


/* Get cell specified by the index */
inline zTCell* zTCells_Get_Cell(zTCells* obj,
			      unsigned int ix)
{
    Z_CHECK_OBJ_PTR(obj);

    if(ix >= obj->z_parent.z_count)
	return NULL;

    return Z_TCELL(obj->z_parent.z_generics_s[ix]);
}

/* Private functions */
/**************************************************************************/

/* Virtual delete function */
static int _ztrow_delete(void* obj, void* usr_data)
{
    zGeneric* zg;
    if(obj)
	{
	    zg = (zGeneric*) obj;
	    zTCell_Delete(Z_TCELL(zg));
	}

    return 0;
}


/* Virtual draw function */
static int _ztrow_draw(void* obj, void* user_data)
{
    zGeneric* zg;
    
    /* check for object */
    Z_CHECK_OBJ(obj);

    zg = (zGeneric*) obj;
    
    /* call draw function */
    return zTCell_Draw(Z_TCELL(zg));
}
