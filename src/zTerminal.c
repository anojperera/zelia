/* Implementation of Terminal class
 * Sat Jan 21 18:06:55 GMT 2012 */

#include "zTerminal.h"

/* Virtual draw function */
static int zterminal_draw_function(zGeneric* obj);


/* Constructor */
zGeneric* zTerminal_New(zTerminal* obj)
{
    if(obj == NULL)
	{
	    obj = (zTerminal*) malloc(sizeof(zTerminal));
	    Z_CHECK_OBJ_PTR(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Create base object */
    if(zBase_New(&obj->z_sbase))
	{
	    if(obj->z_int_flg)
		free(obj);
	    
	    return NULL;
	}
    
    /* set properties */
    obj->z_term_type = zSAK2pt5;
    obj->z_term_height = 0.0;
    obj->z_draw_func = NULL;

    /* set draw function pointer of parent object */
    obj->z_sgeneric.z_draw_func = zterminal_draw_function;

    /* Set child pointer of base object */
    obj->z_sgeneric.z_child = (void*) obj;

    /* Return base pointer */
    return &obj->z_sgeneric.z_sgeneric;
}

/* Destructor */
void zTerminal_Delete(zTerminal* obj)
{
    Z_CHECK_OBJ_VOID(obj);

    obj->z_draw_func = NULL;

    /* delete parent object */
    zBase_Delete(&obj->z_sbase);

    if(obj->z_int_flg)
	free(obj);
}

/* Set terminal type */
inline int zTerminal_Set_Type(zTerminal* obj, zTerminalType type)
{
    Z_CHECK_OBJ(obj);
    obj->z_term_type = type;
    return 0;
}

/* Get terminal type */
inline zTerminalType zTerminal_Get_Type(zTerminal* obj)
{
    Z_CHECK_OBJ(obj);
    return obj->z_term_type;
}

/* Set projected height */
inline int zTerminal_Set_Projected_Height(zTerminal* obj, double height)
{
    Z_CHECK_OBJ(obj);
    obj->z_term_height = height;
}

/* Get projected height */
inline double zTerminal_Get_Projected_Height(zTerminal* obj)
{
    Z_CHECK_OBJ_DOUBLE(obj);
    return obj->z_term_height;
}

/* Draw function */
int zTerminal_Draw(zTerminal* obj)
{
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* Get base and generic object */
    zBase* _base = &obj->z_sbase;
    zGeneric* _genric = &obj->z_sbase.z_sgeneric;

    /* Device context pointer */
    cairo_t* _dev_c = zGeneric_Get_Dev_Context(_genric);

    /* move device context to base coordinates */
    cairo_rectangle(_dev_c,
		    _base->z_x,
		    _base->z_y,
		    _base->z_width,
		    _base->z_height);

    /* cairo stroke to draw */
    cairo_stroke(_dev_c);
}


/*=====================================================================*/
/* Virtual function implementation */
static int zterminal_draw_function(zGeneric* obj)
{
    Z_CHECK_OBJ(obj);
    zTerminal* self = Z_TERMINAL(obj);
    /* call draw function */
    int rt_val = zTerminal_Draw(obj);

    /* Check for child function pointer and call */
    if(self->z_draw_func)
	return self->z_draw_func(obj);
    else
	return rt_val;
}
