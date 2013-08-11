/* Implemenation of generics collection
 * Sun Jan 22 23:48:16 GMT 2012 */

#include "zGenerics.h"

/* Virtual functions */
static void _zgenerics_callback_delete(void* usr_obj, void* obj);						/* delete child objects */
static int _zgenerics_callback_draw(void* usr_obj, void* obj, unsigned int ix);			/* draw child object */
static inline int _zgenerics_del_helper(zGenerics* obj);					/* delete helper */

/* Constructor */
int zGenerics_New(zGenerics* obj,
		  unsigned int s_flg,
		  unsigned int g_count)
{
    int i = 0;
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

    /* Initialise the dynamic collection */
    blist_new(&obj->z_generics_d, NULL);
    blist_set_usr_obj(&obj->z_generics_d, (void*) obj);
    blist_set_option_del_callback(&obj->z_generics_d, _zgenerics_callback_delete);

    obj->z_device = NULL;

    /* if static flag and count was set create the object array
     * as static */
    if(s_flg == 0 && g_count > 0)
	{
	    obj->z_generics_s = (zGeneric**)
		calloc(g_count, sizeof(zGeneric*));

	    /* initialise all objects in array to NULL */
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

/* Clear array */
void zGenerics_Clear(zGenerics* obj)
{
    /* check for object */
    Z_CHECK_OBJ_VOID(obj);
    _zgenerics_del_helper(obj);

    /* If dynamically expansion collection was deleted, recreate it */
    if(obj->z_expansion_flg && blist_count(&obj->z_generics_d))
	{
	    blist_new(&obj->z_generics_d, NULL);
	    blist_set_usr_obj(&obj->z_generics_d, (void*) obj);
	    blist_set_option_del_callback(&obj->z_generics_d, _zgenerics_callback_delete);	    
	}
}

/* Draw function */
int zGenerics_Draw(zGenerics* obj)
{
    int i;
    
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* check for expansion object, if dynamic, use dynamic array */
    if(obj->z_expansion_flg && blist_count(&obj->z_generics_d))
	{
	    blist_foreach(&obj->z_generics_d,
			  0,
			  (void*) obj,
			  _zgenerics_callback_draw);
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
    if(obj->z_expansion_flg && blist_count(&obj->z_generics_d))
	blist_delete(&obj->z_generics_d);
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

    return 0;
}

/* Delete callback function */
static void _zgenerics_callback_delete(void* usr_obj, void* obj)
{
    zGenerics* zg;
    /* indicate failure if objects were NULL */
    if(obj == NULL || usr_obj == NULL)
	return;

    zg = (zGenerics*) usr_obj;

    /* Call delete function pointer of child pointer */
    zg->z_destructor_func(obj, zg->z_usr_data);

    return;
}

/* Draw callback function */
static int _zgenerics_callback_draw(void* usr_obj, void* obj, unsigned int ix)
{
    zGenerics* zg;
    /* indicate failure if objects were NULL */
    if(usr_obj == NULL || obj == NULL)
	return 1;

    zg = (zGenerics*) usr_obj;

    /* Call draw function pointer of child pointer */
    return zg->z_draw_func(obj, zg->z_usr_data);
}
