/* declares all global variables
   Thu Oct  7 22:08:39 BST 2010 */

#ifndef __ZVAR__
#define __ZVAR__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define Z_A3_WIDTH  420
#define Z_A3_HEIGHT 297

#define Z_A4_WIDTH 210
#define Z_A4_HEIGHT 297

#define Z_A3_PAGE_NAME "A3"				/* page size name */
#define Z_A4_PAGE_NAME "A4"				/* page size name */

#define Z_MM_TO_INCHES 25.4				/* conversion inches to mm */
#define Z_POINT_TO_INCHES 72				/* points to inches */

#define Z_FILE_NAME_SZ 512
#define Z_FILE_PATH_SZ 2000

/* file types handled by the system */
typedef enum {
    zFile_Type_SVG,
    zFile_Type_PDF,
    zFile_Type_PNG
} zFile_Type;

/* dimension for calculation */
typedef enum {
    zFile_Dimension_MM,
    zFile_Dimension_CM,
    zFile_Dimension_PT
} zFile_Dimension;


/* error codes */
#define ZELIA_CONSTRUCTED 1
#define ZELIA_INITIALISED 2
#define ZELIA_FILE_FORCE_DEFAULT 3
#define ZELIA_FORCE_OVERWRITE 4

#define ZELIA_OK 0
#define ZELIA_NULL -1
#define ZELIA_FILE_COPY_ERROR -2
#define ZELIA_CACHE_ERROR -3
#define ZELIA_ATTRIB_ERROR -4

/* constructor helper */
#define ZCONSTRUCTOR(obj, type)				\
    if((obj) == NULL)					\
	{						\
	    (obj) = type* malloc(sizeof(type));		\
	    (obj)->_init_flg = ZELIA_CONSTRUCTED;	\
	}						\
    else						\
	(obj)->_init_flg = ZELIA_INITIALISED;		\
    if((obj) == NULL)					\
	return NULL

#define ZCONV_TO_POINTS(val) \
    (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * val

#define ZCONV_TO_RADIANS(val) \
    (M_PI * val / 180.0)

#define ZCONV_TO_PANGO(val) \
    (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * val * PANGO_SCALE


/* Object check macros */
#define ZCHECK_OBJ_VOID(obj)			\
    if((obj) == NULL)				\
	return
#define ZCHECK_OBJ_INT(obj)			\
    if((obj) == NULL)				\
	return 0

#define ZELIA_LOG_MESSAGE(msg)			\
    fprintf(stdout, "%s\n", (msg))
#define ZELIA_LOG_MESSAGE_WITH_INT(msg, val)	\
    fprintf(stdout, "%s%i\n", (msg), (val))
#define ZELIA_LOG_MESSAGE_WITH_STR(msg, val)	\
    fprintf(stdout, "%s%s\n", (msg), (val))
#endif /* __ZVAR__ */
