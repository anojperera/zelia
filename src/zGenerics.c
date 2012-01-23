/* Implemenation of generics collection
 * Sun Jan 22 23:48:16 GMT 2012 */

#include "zGenerics.h"

/* Constructor */
int zGenerics_New(zGenerics** obj,
		  unsigned int s_flg,
		  unsigned int g_count)
{
    /* Check for object */
    Z_CHECK_OBJ(obj);

    /* check for static flag */
    if(s_flg == 0 && g_count == 0)
	{
	    *obj = NULL;
	    return 1;
	}

    /* Create object if NULL */
    if(*obj == NULL)
	{
	    *obj = (zGenerics*) malloc(sizeof(zGenerics));
	    Z_CHECK_OBJ(*obj);
	}

    /* Set properties */
    (*obj)->z_generics_d = NULL;

    /* if static flag and count was set create the object array
     * as static */
    if(s_flg == 0 && g_count > 0)
	{
	    (*obj)->z_generics_s = (zGeneric**)
		calloc(g_count, sizeof(zGeneric*));

	    /* initialise all objects in array to NULL */
	    int i = 0;
	    for(i; i<g_count; i++)
		(*obj)->z_generics_s[i] = NULL;

	    /* set the expansion flag to 0 - indicating not dynamically
	     * expanding */
	    (*obj)->z_expansion_flg = 0;
	}
    
    /* set function pointer to NULL */
    (*obj)->z_draw_func = NULL;

    /* set child pointer to NULL */
    (*obj)->z_child = NULL;
    
    return 0;
}
