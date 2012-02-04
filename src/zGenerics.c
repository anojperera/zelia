/* Implemenation of generics collection
 * Sun Jan 22 23:48:16 GMT 2012 */

#include "zGenerics.h"

/* Virtual functions */
static int _zgenerics_callback_delete(void* usr_obj, void* obj, unsigned int ix);		/* delete child objects */
static int _zgenerics_callback_draw(void* usr_obj, void* obj, unsigned int ix);			/* draw child object */
static inline int _zgenerics_del_helper(zGenerics* obj);						/* delete helper */

/* Constructor */
int zGenerics_New(zGenerics* obj,
		  unsigned int s_flg,
		  unsigned int g_count)
{
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* check for static flag */
    if(s_flg == 0 && g_count == 0)
	{
	    obj = NULL;
	    return 1;
	}

    /* Create object if NULL */
    if(obj == NULL)
	{
	    obj = (zGenerics*) malloc(sizeof(zGenerics));
	    Z_CHECK_OBJ(obj);
	    obj->z_int_flg = 1;
	}
    else
	obj->z_int_flg = 0;

    /* Set properties */
    obj->z_generics_d = NULL;

    obj->z_device = NULL;

    /* if static flag and count was set create the object array
     * as static */
    if(s_flg == 0 && g_count > 0)
	{
	    obj->z_generics_s = (zGeneric**)
		calloc(g_count, sizeof(zGeneric*));

	    /* initialise all objects in array to NULL */
	    int i = 0;
	    for(i=0; i<g_count; i++)
		obj->z_generics_s[i] = NULL;

	    /* set the expansion flag to 0 - indicating not dynamically
	     * expanding */
	    obj->z_expansion_flg = 0;
	    obj->z_count = g_count;
	}
    else
	{
	    /* Initialise to NULL as the collection is dynamically
	     * expandable */
	    obj->z_generics_s = NULL;

	    obj->z_expansion_flg = 1;			/* expansion flag to to dynamically expanding */
	    obj->z_count = 0;				/* counter set to zero */
	}
    
    /* set function pointers to NULL */
    obj->z_draw_func = NULL;
    obj->z_destructor_func = NULL;
    
    /* set child pointer to NULL */
    obj->z_child = NULL;
    obj->z_usr_data = NULL;
    
    return 0;
}

/* Destructor */
void zGenerics_Delete(zGenerics* obj)
{
    /* check for object */
    Z_CHECK_OBJ_VOID(obj);

    
    _zgenerics_del_helper(obj);
    
    if(obj->z_generics_s)
	free(obj->z_generics_s);

    obj->z_generics_s = NULL;
    obj->z_device = NULL;
    obj->z_child = NULL;
    obj->z_usr_data = NULL;
    
    if(obj->z_int_flg)
	free(obj);

    return;
}

/* Return count of itnernal object */
inline unsigned int zGenerics_Get_Count(zGenerics* obj)
{
    if(obj == NULL)
	return 0;
    else
	return obj->z_count;
}

/* Set device */
inline int zGenerics_Set_Device(zGenerics* obj, zDevice* dev)
{
    Z_CHECK_OBJ(obj);
    Z_CHECK_OBJ(dev);

    obj->z_device = dev;
    return 0;
}

/* Get device */
inline zDevice* zGenerics_Get_Device(zGenerics* obj)
{
    Z_CHECK_OBJ_PTR(obj);
    return obj->z_device;
}

/* Clear array */
void zGenerics_Clear(zGenerics* obj)
{
    /* check for object */
    Z_CHECK_OBJ_VOID(obj);
    _zgenerics_del_helper(obj);
}

/* Draw function */
int zGenerics_Draw(zGenerics* obj)
{
    int i;
    
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* check for expansion object, if dynamic, use dynamic array */
    if(obj->z_expansion_flg && obj->z_generics_d)
	{
	    aList_Display2(&obj->z_generics_d,
			  0,
			   _zgenerics_callback_draw,
			   (void*) obj);
	}
    else if(obj->z_generics_s)
	{
	    for(i=0; i<obj->z_count; i++)
		{
		    if(obj->z_draw_func && obj->z_generics_s[i])
			obj->z_draw_func((void*) obj->z_generics_s[i], obj->z_usr_data);
		}
	}

    return 0;
    
}

/*================================================================*/
/* Private functions */

/* Delete and clear function helper */
static inline int _zgenerics_del_helper(zGenerics* obj)
{
    int i;
    /* check for expansion flag */
    if(obj->z_expansion_flg)
	{
	    /* call destructors of child objects */
	    aList_Display2(&obj->z_generics_d,
			  0,
			   _zgenerics_callback_delete,
			   (void*) obj);

	    /* clear internal list */
	    aList_Clear(&obj->z_generics_d);
	}
    else
	{
	    /* iterate through static collection and destructors */
	    for(i=0; i<obj->z_count; i++)
		{
		    if(obj->z_destructor_func && obj->z_generics_s[i])
			obj->z_destructor_func((void*) obj->z_generics_s[i], NULL);
		    obj->z_generics_s[i] = NULL;
		}
	}
    obj->z_count = 0;
    obj->z_generics_d = NULL;

    return 0;
}

/* Delete callback function */
static int _zgenerics_callback_delete(void* usr_obj, void* obj, unsigned int ix)
{

    /* indicate failure if objects were NULL */
    if(usr_obj == NULL || obj == NULL)
	return 1;

    zGenerics* zg = (zGenerics*) usr_obj;

    /* Call delete function pointer of child pointer */
    return zg->z_destructor_func(obj, zg->z_usr_data);
}

/* Draw callback function */
static int _zgenerics_callback_draw(void* usr_obj, void* obj, unsigned int ix)
{
    /* indicate failure if objects were NULL */
    if(usr_obj == NULL || obj == NULL)
	return 1;

    zGenerics* zg = (zGenerics*) usr_obj;

    /* Call draw function pointer of child pointer */
    return zg->z_draw_func(obj, zg->z_usr_data);
}
