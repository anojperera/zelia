/* Header file for declaraing interfaces for junction boxes.
 * inherits the base class.
 * JB classes can be associated with a collection of cable glands
 * and terminals
 * Mon Jan 30 23:05:29 GMT 2012 */

#ifndef _ZJB_H_
#define _ZJB_H_

#include <stdlib.h>
#include "zdevice.h"
#include "zbase.h"
#include "zgeneric.h"
#include "zgenerics.h"

/* Forward declaration of struct */
typedef struct _zjb zjb;

struct _zjb
{
    zbase parent;				/* Inherited parent object */
    unsigned int _init_flg;			/* Internal flag */
    unsigned int terms_ext;			/* Flag to indicate terminal collection
						 * externally set */
    unsigned int glands_ext;			/* Flag to indicate glads collection external */
    double depth;				/* Depth of junction box */
    double ang;					/* Orientation angle */
    double rad;					/* radius */
    zgenerics* terms;				/* Terminals collection */
    zgenerics* glands;				/* cable gland collection */

    struct _zgeneric_vtable vtable;		/* vtable */
    zgeneric* super_cls;			/* super class pointer */
    void* child;				/* child pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and Destructor */
    zgeneric* zjb_new(zjb* obj,
		      zdevice* dev,
		      double x,
		      double y,
		      double width,
		      double height,
		      double depth,
		      double ang);
    void zjb_delete(zjb* obj);

    /* draw function */
    int zjb_draw(zjb* obj);

    /* Property methods */
    /***********************************************************************/
    /* Set and get fillet radius */
    inline __attribute__ ((always_inline)) static int zjb_set_fillet_radius(zjb* obj, double rad)
    {
	/* Check for object */
	ZCHECK_OBJ_INT(obj);
	obj->rad = rad;
	return 0;
    }
    inline __attribute__ ((always_inline)) static double zjb_get_fillet_radius(zjb* obj)
    {
	ZCHECK_OBJ_INT(obj);
	return obj->rad;
    }

    /* Set and get terminals collection */
    inline __attribute__ ((always_inline)) static int zjb_set_terminals(zjb* obj, zgenerics* terms)
    {
	/* check for objects */
	ZCHECK_OBJ_INT(obj);
	ZCHECK_OBJ_INT(terms);

	if(obj->terms_ext == 0)
	    return ZELIA_JB_ERROR;
    
	obj->terms = terms;

	/* indicate external flag was set */
	obj->terms_ext = 1;
	return ZELIA_OK;
    }
    
    inline __attribute__ ((always_inline)) static zgenerics* zjb_get_terminals(zjb* obj)
    {
	ZCHECK_OBJ_PTR(obj);
	return obj->terms;
    }

    /* Add terminal collection */
    int zjb_add_terminals(zjb* obj,
			  unsigned int num_term,			/* number of terminals */
			  double width,					/* terminal width */
			  double height,				/* terminal height */
			  const char* links);				/* terminal links */
    
    /* Set and get gland collection */
    inline __attribute__ ((always_inline)) static int zjb_set_glands(zjb* obj, zgenerics* glands)
    {
	ZCHECK_OBJ_INT(obj);
	ZCHECK_OBJ_INT(glands);

	if(obj->glands == 0)
	    return ZELIA_JB_ERROR;
    
	obj->glands = glands;
	obj->glands_ext = 1;

	return ZELIA_OK;
    }
    
    inline __attribute__ ((always_inline)) static zgenerics* zjb_get_glands(zjb* obj)
    {
	/* Check for object */
	ZCHECK_OBJ_PTR(obj);
	return obj->glands;
    }

    /* Adds a gland into the internal collection.
     * coordinates are relative to the JB */
    int zjb_add_glands(zjb* obj,
		       double x,					/* relative coordinate */
		       double y,					/* relative coordinate */
		       zGlandSize sz,					/* gland size */
		       unsigned int hex_flg);				/* hex flag */

#ifdef __cplusplus
}
#endif

#endif /* _ZJB_H_ */
