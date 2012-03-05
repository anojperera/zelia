/* Implementation zTRows collection */
/* Mon Mar  5 21:16:27 GMT 2012 */

#include "zTRows.h"

/* Virtual functions */
static int _ztrows_delete(void* obj, void* usr_data);	/* delete function */
static int _ztrows_draw(void* obj, void* usr_data);	/* draw function */

/* Constructor */
zGenerics* zTRows_New(zTRows* obj,			/* optional argument */
			  zDevice* dev,			/* device object */
			  unsigned int num_rows,	/* number of rows */
			  double x,			/* origin x */
			  double y,			/* origin y */
			  double width,			/* width */
			  double height)		/* height */
{
    int i;
    zTRow* _trow;
    
    if(num_rows == 0)
	return NULL;
    
    /* check object */
    if(obj == NULL)
	{
	    obj = (zTRows*) malloc(sizeof(zTRows));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 1;

    /* create parent */
    if(zGenerics_New(&obj->z_parent,
		     0,
		     num_rows))
	{
	    if(obj->z_int_flg)
		free(obj);
	    return NULL;
	}

    /* set device */
    zGenerics_Set_Device(&obj->z_parent, dev);

    /* create object array */
    for(i=0; i<num_col; i++)
	{
	    /* create object */
	    obj->z_parent.z_generics_s[i] =
		zTRow_New(NULL, i);

	    /* set width and height */
	    zBase_Set_Width_and_Height(Z_BASE(obj->z_parent.z_generics_s[i]),
				       width,
				       height);
	    zBase_Set_Base_Coords(Z_BASE(obj->z_parent.z_generics_s[i]),
				  x,
				  y + (double) height);
	    
	    /* Default device context is set by TCells */
	}

    /* Set function pointers of parent object */
    obj->z_parent.z_draw_func = _ztrows_draw;
    obj->z_parent.z_destructor_func = _ztrows_delete;

    /* Set child pointer and user data of parent */
    obj->z_parent.z_child = (void*) obj;
    obj->z_parent.z_usr_data = (void*) obj;

    /* return parent object */
    return &obj->z_parent;
}

/* Destructor */
void zTRows_Delete(zTRows* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    /* call delete method of parent object */
    zGenerics_Delete(&obj->z_parent);

    obj->z_child = NULL;
    obj->z_draw_func = NULL;

    if(obj->z_int_flg)
	free(obj);
}

/* Get row from collection */
zTRow* zTRows_Get_Row(zTRows* obj, unsigned int ix)
{
    Z_CHECK_OBJ_PTR(obj);
    if(ix > obj->z_parent.z_count)
	return NULL;

    return Z_TROW(&obj->z_parent.z_generics_s[ix]);
}

/* Private methods */
/****************************************************************/

/* Virtual delete method */
static int _ztrows_delete(void* obj, void* usr_data)
{
    zGeneric* zg;
    if(obj)
	{
	    zg = (zGeneric*) obj;
	    zTRow_Delete(Z_TROW(zg));
	}

    return 0;
}

/* Virtual draw method */
static int _ztrows_draw(void* obj, void* usr_data)
{
    zGeneric* zg;

    /* Check for object */
    Z_CHECK_OBJ(obj);
    zg = (zGeneric*) obj;

    /* call draw function */
    return zTRow_Draw(Z_TROW(zg));
}
