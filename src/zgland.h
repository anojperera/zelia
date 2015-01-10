/* Header file for describing interfaces for cable glands
 * inherits base object.
 * Fri Feb  3 13:40:12 GMTST 2012 */

#ifndef __ZGLAND_H__
#define __ZGLAND_H__

#include <stdlib.h>
#include "zdevice.h"
#include "zgeneric.h"
#include "zbase.h"

/* Forward declaration of struct */
typedef	struct _zgland zgland;

struct _zgland
{
    zbase parent;				/* inherited parent object */
    unsigned int _init_flg;			/* internal flag */
    unsigned int hex_flg;			/* flag draw hex */
    double dia;					/* diameter */
    zGlandSize gland_size;			/* gland size */

    void* child;				/* child object */
    zgeneric* super_cls;			/* super class pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgeneric* zgland_new(zgland* obj,		/* optional object */
			 zdevice* dev,		/* optional device object */
			 double x,
			 double y,
			 zGlandSize sz);	/* size parameter */

    void zgland_delete(zgland* obj);


    /* Draw method */
    int zgland_draw(zgland* obj);

    /************************************************/
    /* Property methods */

    /* Set and get gland diameter */
    inline __attribute__ ((always_inline)) static int zgland_set_gland_dia(zgland* obj, double dia)
    {
	ZCHECK_OBJ_INT(obj);
	obj->dia = dia;
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static double zgland_get_gland_dia(zgland* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->dia;
    }

    /* Set and get gland size */
    inline __attribute__ ((always_inline)) static int zgland_set_gland_size(zgland* obj, zGlandSize sz)
    {
	ZCHECK_OBJ_INT(obj);
	obj->gland_size = sz;
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static zGlandSize zgland_get_gland_size(zgland* obj)
    {
	if(obj == NULL)
	    return zM20;
	return obj->gland_size;
    }

    /* Set and get hex flag */
    /* Hex flag indicates the hex profile of a gland */
    inline __attribute__ ((always_inline)) static int zgland_set_hex_profile_flag(zgland* obj, unsigned int flg)
    {
	ZCHECK_OBJ_INT(obj);
	obj->hex_flg = flg;
	return ZELIA_OK;
    }
    inline __attribute__ ((always_inline)) static unsigned int zgland_get_hex_profile_flag(zgland* obj)
    {
	ZCHECK_OBJ_INT(obj);
	return obj->hex_flg;
    }


#ifdef __cplusplus
}
#endif

#endif /* __ZGLAND_H__ */
