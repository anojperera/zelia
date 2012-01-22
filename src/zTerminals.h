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

/* Forward declaration of struct */
typedef struct _zTerminals zTerminals;

struct _zTerminals
{
    zTerminal** z_terminals;				/* Collection of terminals */
    unsigned int z_count;				/* count of terminals */
    /* Link string to be specified in the following format;
     * 1-2, 4-8 etc. */
    char z_term_links[32];				/* 32 byte long link string.
    
    
};

#endif /* _ZTERMINALS_H_ */
