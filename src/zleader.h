/* Header file describes interfaces for creating a leader.
 * Inherits arrow class. 256 char buffer is provided for
 * storing a description for the leader.
 * Tue Feb  7 15:50:50 GMTST 2012 */

#ifndef __ZLEADER_H__
#define __ZLEADER_H__

#include <stdlib.h>
#include "zarrow.h"
#include "zgeneric.h"
#include "zdevice.h"

#define Z_LEADER_DESC_BUFF 256

/* Forward declaration of struct */
typedef struct _zleader zleader;

struct _zleader
{
    zarrow parent;						/* inherited parent object */
    unsigned int _init_flg;					/* internal flag */
    double length_1;						/* first segment length */
    double length_2;						/* second segment length */

    char content[LEADER_DESC_BUFF];				/* leader description / content */

    struct _zgeneric_vtable vtable;				/* vtable */
    void* child;						/* child pointer */
    zgeneric* super_cls;					/* super class pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    zgeneric* zleader_new(zleader* obj,				/* leader object optional */
			  zdevice* dev,				/* device object optional */
			  double x,			        /* base coordinate x */
			  double y,				/* base coordinate y */
			  double seg_1,				/* first segment length */
			  double seg_2,				/* second segment length */
			  double ang,				/* orientation angle */
			  const char* desc);			/* description */

    void zleader_delete(zleader* obj);

    /* Draw function */
    int zleader_draw(zleader* obj);

    /**************************************************************************************/
    /* Property methods */

    /* Set and get segment lengths */
    inline __attribute__ ((always_inline)) static int zleader_set_segment_lengths(zleader* obj, double seg_1, double seg_2)
    {
	ZCHECK_OBJ_INT(obj);
	obj->length_1 = seg_1;
	obj->length_1 = seg_2;

	return ZELIA_OK;
    }
    
    inline __attribute__ ((always_inline)) static double zleader_get_first_segment(zleader* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->length_1;
    }
    
    inline __attribute__ ((always_inline)) static double zleader_get_second_segment(zleader* obj)
    {
	ZCHECK_OBJ_DOUBLE(obj);
	return obj->length_2;
    }

    /* Set and get description */
    inline __attribute__ ((always_inline)) static int zleader_set_description(zleader* obj, const char* desc)
    {
	ZCHECK_OBJ_INT(obj);
	ZCHECK_OBJ_INT(desc);

	strcpy(obj->content, desc);
	return ZELIA_OK;
    }
    
    inline __attribute__ ((always_inline)) static const char* zleader_get_description(zleader* obj)
    {
	ZCHECK_OBJ_PTR(obj);
	return obj->z_content;	
    }
#ifdef __cplusplus
}
#endif

#endif /* __ZLEADER_H__ */
