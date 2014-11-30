/* Header file for declaring interface for creating a terminal
 * stack. Uses a collection of zTerminal class.
 * Provides interfaces for number of terminals, number,
 * barriers, bridges.
 *****************************************************************
 * The interface lets the user specify bridging between terminals
 * as an index based string (1-2, 5-9). The string shall be decoded
 * and links are added. 
 * Sun Jan 22 15:04:38 GMT 2012 */

#ifndef _ZTERMINALS_H_
#define _ZTERMINALS_H_

#include <stdlib.h>
#include "zgenerics.h"

#define ZTERMINALS_LK_SZ 32

/* Forward declaration of struct */
typedef struct _zterminals zterminals;

struct _zterminals
{
    zgenerics parent;					/* inherited parent object */
    unsigned int int_flg;				/* internal flag */
    unsigned int links_flg;				/* links flag */

    int _d_counter;					/* terminal counter */
    
    /* Link string to be specified in the following format;
     * 1-2, 4-8 etc. */
    char term_links[ZTERMINALS_LK_SZ];			/* 32 byte long link string. */
    double x;						/* x coord */
    double y;						/* y coord */
    double width;					/* width */
    double height;					/* height */
    double ang;						/* angle */
    double* x_links;					/* link coordinates array */
    double* y_links;					/* link coordinates array */

    struct _zgeneric_vtable vtable;			/* vtable */
    zgenerics* super_cls;				/* super class pointer */
    void* child;					/* child class pointer */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zgenerics* zterminals_new(zterminals* obj,
			      zdevice* dev,		/* device object */
			      unsigned int num_term,	/* number of terminals */
			      double x,			/* base coordinate */
			      double y,			/* base coordinate */
			      double width,		/* width */
			      double height,		/* height */
			      double ang,		/* orientation angle */
			      const char* links);	/* links */

    /* Destruct is a virtual interface */
    void zterminals_delete(zterminals* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZTERMINALS_H_ */
