/* Implementation of zTCells class */
/* Sun Mar  4 12:57:34 GMT 2012 */

#include "ztcells.h"
#include "zVar.h"

/* Virtual functions */
static int _ztcell_delete(void* obj);				/* delete function */
static int _ztcell_draw(void* obj);				/* draw function */

/* Constructor */
zgenerics* ztcells_new(ztcells* obj,				/* Optional argument */
		       zdevice* dev,				/* Device object */
		       unsigned int row_ix,			/* row index */
		       unsigned int num_col,			/* Number of columns */
		       double x,				/* origin x */
		       double y,				/* origin y */
		       double col_width,			/* Column width */
		       double row_height)			/* Row height */
{
    int _i;
    ztcell* _tcell;

    /* create object */
    ZCONSTRUCTOR(obj, ztcells);

    /* Create parent object */
    if(!(obj->super_cls = zgenerics_new(&obj->parent,
				       0,
					num_col)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set device */
    zgenerics_set_device(&obj->parent, dev);

    /* Set properties */
    obj->width = col_width;
    obj->height = row_height;
	
    obj->child = NULL;

    /* Create object array */
    for(_i=0; _i<num_col; _i++)
	{
	    obj->parent.generics_s[_i] = ztcell_new(NULL);

	    /* set collection pointer to element */
	    zgeneric_set_collection_pointer(obj->parent.generics_s[_i], (void*) obj);
	    
	    _tcell = Z_TCELL(obj->parent.generics_s[_i]);

	    /* Set index, width and height */
	    _tcell->_row_ix = row_ix;
	    _tcell->_col_ix = (unsigned int) _i;

	    /* Set base coordinates */
	    zbase_set_base_coords(Z_BASE(obj->parent.generics_s[_i]),
				  x + (double) _i * col_width,
				  y);
	    
	    /* Set width and height of cell */
	    zbase_set_width_and_height(Z_BASE(obj->parent.generics_s[_i]),
				       col_width,
				       row_height);

	    /* Set default device context */
	    zgeneric_set_device(obj->parent.generics_s[_i],
				zgenerics_get_device(&obj->parent));
	    
	    /* Set default device context */
	    zgeneric_set_default_dev_context(obj->parent.generics_s[_i]);
	    
	    /* Set line flag */
	    ztcell_set_line_flg(_tcell, 1);
	}

    /* Set parent properties */
    zgeneric_set_delete_callback(obj, _ztcell_delete);
    zgeneric_set_draw(obj, _ztcell_draw);

    /* Parent user data set as self */
    obj->parent.usr_data = (void*) obj;

    /* Set child poitner of parent */
    zgeneric_set_child_pointer(obj);

    return obj->super_cls;
}

/* Destructor */
void ztcells_delete(ztcells* obj)
{
    /* check object */
    ZCHECK_OBJ_VOID(obj);

    if(obj->vtable.zgeneric_delete)
	obj->vtable.zgeneric_delete((void*) obj->super_cls);
	
    /* call delete method of parent object */
    zgenerics_delete(&obj->parent);

    obj->child = NULL;
    obj->super_cls = NULL;
    ZGENERIC_INIT_VTABLE(obj);

    /* if the object was created we free it */
    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}


/*=================================== Private Methods ===================================*/

/* Virtual delete function */
static int _ztcell_delete(void* obj)
{
    zgeneric* _zg = NULL;
	
    ZCHECK_OBJ_INT(obj);
	
    _zg = (zgeneric*) obj;
    ztcell_delete(Z_TCELL(_zg));

    return ZELIA_OK;
}


/* Virtual draw function */
static int _ztcell_draw(void* obj)
{
    zgeneric* _zg = NULL;
    
    /* check for object */
    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;
    
    /* call draw function */
    return ztcell_draw(Z_TCELL(_zg));
}
