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

    

}
