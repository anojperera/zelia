/* Implementation of zTRow class */
/* Sun Mar  4 18:00:47 GMT 2012 */

#include "ztrow.h"

/* Virtual function */
static int _ztrow_draw(void* obj);


/* Constructor */
zgeneric* ztrow_new(ztrow* obj, unsigned int ix)
{
    /* call constructor helper */
    ZCONSTRUCTOR(obj, ztrow);

    /* Create base object */
    if(!(obj->super_cls = zbase_new(&obj->parent)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* Set properties */
    obj->arr_flg = 0;				/* Flag to indicate tcells array not created */
    obj->num_cols = 0;				/* number of columns */
    obj->child = NULL;
    obj->ix = ix;

    /* Set function and child pointer of parent object */
    /* initialise vtable */
    ZGENERIC_INIT_VTABLE(obj);

    /* set parent objects function pointers */
    zgeneric_set_draw(obj, _ztrow_draw);
    
    /* set child pointer of parent object */
    zgeneric_set_child_pointer(obj);

    /* Return top level object */
    return obj->super_cls;
}

/* Destructor */
void ztrow_delete(ztrow* obj)
{
    ZCHECK_OBJ_VOID(obj);

    /* if the destructor callback was set call it */
    if(obj->vtable.zgeneric_delete)
	obj->vtable.zgeneric_delete((void*) obj->super_cls);
    else
	{
	    /* delete parent object */
	    zbase_delete(&obj->parent);
	}

    obj->super_cls = NULL;
    obj->child = NULL;

    ZGENERIC_INIT_VTABLE(obj);

    /* if tcells was created destroy */
    if(obj->arr_flg)
	ztcells_delete(&obj->tcells);

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Draw function */
int ztrow_draw(ztrow* obj)
{
    zdevice* _dev;
    zgeneric* _zg;
    zgenerics* _zgs;
    zbase* _base;

    /* check for object */
    ZCHECK_OBJ_INT(obj);

    /* if number of columns were not set exit */
    if(obj->num_cols == 0)
	return ZELIA_TROW_ERROR;

    _zg = Z_GENERIC(obj);
    _base = &obj->parent;
    _dev = zgeneric_get_device(_zg);

    /* check if tcell collection was created or not */
    if(obj->arr_flg == 0)
	{
	    ztcells_new(&obj->tcells,
			_dev,
			obj->ix,
			obj->num_cols,
			_base->x,
			_base->y,
			_base->width / (double) obj->num_cols,
			_base->height);
	    obj->arr_flg = 1;
	}

    _zgs = Z_GENERICS(&obj->tcells);
    return zgenerics_draw(_zgs);
}

/* Add content to the cell */
inline int ztrow_add_content(ztrow* obj,
			     unsigned int ix,						/* Column index */
			     const char* content)					/* Content */
{
    zgenerics* _zgs;
    zdevice* _dev;

    /* check object */
    ZCHECK_OBJ_INT(obj);

    /* check index is over bound */
    if(ix > obj->num_cols)
	return ZELIA_TROW_ERROR;

    /* check if tcell collection was created or not */
    if(obj->arr_flg == 0)
	{
	    _dev = zgeneric_get_device(Z_GENERIC(obj));
	    ztcells_new(&obj->tcells,
			_dev,
			obj->ix,
			obj->num_cols,
			obj->parent.x,
			obj->parent.y,
			obj->parent.width / (double) obj->num_cols,
			obj->parent.height);
	    obj->arr_flg = 1;
	}

    _zgs = Z_GENERICS(&obj->tcells);
    return ztcell_set_content(Z_TCELL(_zgs->generics_s[ix]), content);
}

/*=================================== Property Methods ===================================*/

/* Get Cell */
ztcell* ztrow_get_cell(ztrow* obj, unsigned int ix)
{
    zgenerics* _zg;
    zdevice* _dev;

    ZCHECK_OBJ_PTR(obj);

    /* check array bounds */
    if(ix > obj->num_cols)
	return NULL;

    if(obj->arr_flg == 0)
	{
	    _dev = zgeneric_get_device(Z_GENERIC(obj));
	    ztcells_new(&obj->tcells,
			_dev,
			obj->ix,
			obj->num_cols,
			obj->parent.x,
			obj->parent.y,
			obj->parent.width / (double) obj->num_cols,
			obj->parent.height);
	    obj->arr_flg = 1;
	}
    _zg = Z_GENERICS(&obj->tcells);
    return Z_TCELL(_zg->generics_s[ix]);
}


/*=================================== Private Methods ===================================*/

/* Virtual draw method */
static int _ztrow_draw(void* obj)
{
    int _rt = ZELIA_OK;
    zgeneric* _zg = NULL;
    ztrow* _self = NULL;

    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;
    _self = Z_TROW(_zg);
    _rt =  ztrow_draw(_self);

    /* if child pointer draw callback was set we call it */
    if(_self->vtable.zgeneric_draw)
	return _self->vtable.zgeneric_draw(obj);

    return _rt;
}
