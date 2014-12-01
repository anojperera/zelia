/* Implementation zTRows collection */
/* Mon Mar  5 21:16:27 GMT 2012 */

#include "ztrows.h"

/* Virtual functions */
static int _ztrows_delete(void* obj);			/* delete function */
static int _ztrows_draw(void* obj);			/* draw function */

/* Constructor */
zgenerics* ztrows_new(ztrows* obj,			/* optional argument */
		      zdevice* dev,			/* device object */
		      unsigned int num_rows,		/* number of rows */
		      unsigned int num_cols,		/* number of columns */
		      double x,				/* origin x */
		      double y,				/* origin y */
		      double width,			/* width */
		      double height)			/* height */
{
    int _i;

    if(num_rows == 0)
	return NULL;


    ZCONSTRUCTOR(obj, ztrows);

    /* create parent */
    if(!(obj->super_cls = zgenerics_new(&obj->parent,
					0,
					num_rows)))
	{
	    if(ZDESTRUCTOR_CHECK)
		free(obj);
	    return NULL;
	}

    /* set device */
    zgenerics_set_device(&obj->parent, dev);

    /* create object array */
    for(_i=0; _i<num_rows; _i++)
	{
	    /* create object */
	    obj->parent.generics_s[_i] = ztrow_new(NULL, _i);

	    /* set collection pointer */
	    zgeneric_set_collection_pointer(obj->parent.generics_s[_i], (void*) obj);

	    /* set width and height */
	    zbase_set_width_and_height(Z_BASE(obj->super_cls->generics_s[_i]),
				       width,
				       height);
	    zbase_set_base_coords(Z_BASE(obj->super_cls->generics_s[_i]),
				  x,
				  y + (double) _i * height);
	    ztrow_set_num_cols(Z_TROW(obj->super_cls->generics_s[_i]),
			       num_cols);
	    zgeneric_set_device(obj->super_cls->generics_s[_i], dev);
	    /* Default device context is set by TCells */
	}

    /* initialise function pointers */
    ZGENERIC_INIT_VTABLE(obj);

    /* Set function pointers of parent object */
    zgeneric_set_delete_callback(obj, _ztrows_delete);
    zgeneric_set_draw(obj, _ztrows_draw);

    /* Set child pointer and user data of parent */
    zgeneric_set_child_pointer(obj);
    obj->parent.usr_data = (void*) obj;

    /* return parent object */
    return obj->super_cls;
}

/* Destructor */
void ztrows_delete(ztrows* obj)
{
    ZCHECK_OBJ_VOID(obj);

    /* if the destructor callback was set we call it */
    if(obj->vtable.zgeneric_delete)
	obj->vtable.zgeneric_delete((void*) obj->super_cls);

    /* call delete method of parent object */
    zgenerics_delete(obj->super_cls);

    obj->child = NULL;
    obj->super_cls = NULL;

    /* remove callbacks */
    ZGENERIC_INIT_VTABLE(obj);

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/*=================================== Private Methods ===================================*/

/* Virtual delete method */
static int _ztrows_delete(void* obj)
{
    zgeneric* _zg;

    ZCHECK_OBJ_INT(obj);
    _zg = (zgeneric*) obj;

    ztrow_delete(Z_TROW(_zg));

    return ZELIA_OK;
}

/* Virtual draw method */
static int _ztrows_draw(void* obj)
{
    zgeneric* _zg;

    /* Check for object */
    ZCHECK_OBJ_INT(obj);

    _zg = (zgeneric*) obj;

    /* call draw function */
    return ztrow_draw(Z_TROW(_zg));
}
