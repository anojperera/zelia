/* Implemenation of generics collection
 * Sun Jan 22 23:48:16 GMT 2012 */

#include "zgenerics.h"

/* callback methods */
static void _zgenerics_callback_delete(void* usr_obj, void* obj);				/* delete child objects */
static int _zgenerics_callback_draw(void* usr_obj, void* obj, unsigned int ix);			/* draw child object */
static inline int _zgenerics_del_helper(zgenerics* obj);					/* delete helper */

/* Constructor */
zgenerics* zgenerics_new(zgenerics* obj,
		  unsigned int s_flg,
		  unsigned int g_count)
{
    int i = 0;
    /* Check for object */
    ZCHECK_OBJ_INT(obj);

    /* check for static flag */
    if(s_flg == 0 && g_count == 0)
	{
	    obj = NULL;
	    return NULL;
	}

    /* call constructor */
    ZCONSTRUCTOR(obj, zgenerics);

    /* Set properties */

    /* Initialise the dynamic collection */
    blist_new(&obj->generics_d, NULL);
    blist_set_usr_obj(&obj->generics_d, (void*) obj);
    blist_set_option_del_callback(&obj->generics_d, _zgenerics_callback_delete);

    obj->device = NULL;

    /* if static flag and count was set create the object array
     * as static */
    if(s_flg == 0 && g_count > 0)
	{
	    obj->generics_s = (zgeneric**)
		calloc(g_count, sizeof(zgeneric*));

	    /* initialise all objects in array to NULL */
	    for(i=0; i<g_count; i++)
		obj->generics_s[i] = NULL;

	    /* set the expansion flag to 0 - indicating not dynamically
	     * expanding */
	    obj->expansion_flg = 0;
	    obj->count = g_count;
	}
    else
	{
	    /* Initialise to NULL as the collection is dynamically
	     * expandable */
	    obj->generics_s = NULL;

	    obj->expansion_flg = 1;			/* expansion flag to to dynamically expanding */
	    obj->count = 0;				/* counter set to zero */
	}

    obj->ref_flg = 0;
    
    /* initialise the vtable */
    ZGENERIC_INIT_VTABLE(obj);
    
    /* set child pointer to NULL */
    obj->child = NULL;
    obj->usr_data = NULL;

    return obj;
}

/* Destructor */
void zgenerics_delete(zgenerics* obj)
{
    /* check for object */
    ZCHECK_OBJ_VOID(obj);

    if(obj->vtable.(obj)->vtable.zgeneric_delete)
	obj->vtable.(obj)->vtable.zgeneric_delete(void* obj);

    _zgenerics_del_helper(obj);

    if(obj->generics_s)
	free(obj->generics_s);
    
    zdevice_delete(obj->device);
    
    obj->generics_s = NULL;
    obj->device = NULL;
    obj->child = NULL;
    obj->usr_data = NULL;
    obj->count = 0;
    obj->ref_flg = 0;
    
    /* remove vtables */
    ZGENERIC_INIT_VTABLE(obj);

    if(ZDESTRUCTOR_CHECK)
	free(obj);

    return;
}

/* Clear array */
void zgenerics_clear(zgenerics* obj)
{
    /* check for object */
    ZCHECK_OBJ_VOID(obj);
    _zgenerics_del_helper(obj);

    /* If dynamically expansion collection was deleted, recreate it */
    if(obj->expansion_flg && blist_count(&obj->generics_d))
	{
	    blist_new(&obj->generics_d, NULL);
	    blist_set_usr_obj(&obj->generics_d, (void*) obj);
	    blist_set_option_del_callback(&obj->generics_d, _zgenerics_callback_delete);
	}
    obj->count = 0;
    
    return;
}

/* Draw function */
int zgenerics_draw(zgenerics* obj)
{
    int i = 0;

    /* Check for object */
    ZCHECK_OBJ_INT(obj);

    /* check for expansion object, if dynamic, use dynamic array */
    if(obj->expansion_flg && blist_count(&obj->generics_d))
	{
	    blist_foreach(&obj->generics_d,
			  0,
			  (void*) obj,
			  _zgenerics_callback_draw);
	}
    else if(obj->generics_s)
	{
	    for(i=0; i<obj->count; i++)
		{
		    if(obj->vtable.zgeneric_draw && obj->generics_s[i])
			obj->vtable.zgeneric_draw((void*) obj->generics_s[i]);
		}
	}

    return ZELIA_OK;

}

/*================================================================*/
/* Private functions */

/* Delete and clear function helper */
static inline int _zgenerics_del_helper(zgenerics* obj)
{
    int _i;
    /* check for expansion flag */
    if(obj->expansion_flg && blist_count(&obj->generics_d))
	blist_delete(&obj->generics_d);
    else
	{
	    /* iterate through static collection and destructors */
	    for(_i=0; _i<obj->count; _i++)
		{
		    if(obj->vtable.zgeneric_delete && obj->generics_s[_i])
			obj->vtable.zgeneric_delete((void*) obj->generics_s[_i]);
		    obj->generics_s[_i] = NULL;
		}
	}
    obj->count = 0;

    return 0;
}

/* Delete callback function */
static void _zgenerics_callback_delete(void* usr_obj, void* obj)
{
    zgenerics* _zg;
    /* indicate failure if objects were NULL */
    if(obj == NULL || usr_obj == NULL)
	return;

    _zg = (zgenerics*) usr_obj;

    /* Call delete function pointer of child pointer */
    if(_zg->vtable.zgeneric_delete)
	_zg->vtable.zgeneric_delete(obj);

    return;
}

/* Draw callback function */
static int _zgenerics_callback_draw(void* usr_obj, void* obj, unsigned int ix)
{
    zgenerics* _zg;
    /* indicate failure if objects were NULL */
    if(usr_obj == NULL || obj == NULL)
	return 1;

    _zg = (zgenerics*) usr_obj;

    /* Call draw function pointer of child pointer */
    return _zg->vtable.zgeneric_draw(obj);
}
