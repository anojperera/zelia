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
#include "zGenerics.h"

/* Forward declaration of struct */
typedef struct _zTerminals zTerminals;

struct _zTerminals
{
    zGenerics var_parent;				/* inherited parent object */
    unsigned int z_count;				/* count of terminals */
    /* Link string to be specified in the following format;
     * 1-2, 4-8 etc. */
    char z_term_links[32];				/* 32 byte long link string. */
};

#ifdef __cplusplus
extern "C" {
#endif

    /* Constructor and destructor */
    zGenerics* zTerminals_New(zTerminals* obj,
			      unsigned int num_term,	/* number of terminals */
			      double x,			/* base coordinate */
			      double y,			/* base coordinate */
			      double width,		/* width */
			      double height,		/* height */
			      double ang,		/* orientation angle */
			      const char* links);	/* links */

    /* Destruct is a virtual interface */
    void zTerminals_Delete(zTerminals* obj);

#ifdef __cplusplus
}
#endif

#endif /* _ZTERMINALS_H_ */
