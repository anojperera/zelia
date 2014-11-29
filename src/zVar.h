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

#define Z_TEMP_FILE_DIR "/tmp/"

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

/* line colours */
typedef enum {
    zLRed,
    zLBlue,
    zLYellow,
    zLCyan,
    zLBlack
} zLineColour;

/* Page sizes */
typedef enum {
    zSheetA4_Portrait,
    zSheetA4_Landscape,
    zSheetA3_Portrait,
    zSheetA3_Landscape
} zSheets;

/* line weights */
typedef enum {
    zLWeight1,
    zLWeight2,
    zLWeight3,
    zLWeight4,
    zLWeight5,
    zLWeight6,
    zLWeight7,
    zLWeight8
} zLineWeights;

/* line types */
typedef enum {
    zLTContinuous,
    zLTCenter,
    zLTHidden
} zLineTypes;

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
#define ZELIA_EMPTY_FILE -5
#define ZELIA_TCELL_ERROR -6
#define ZELIA_TROW_ERROR -7
#define ZELIA_TABLE_ERROR -8

/* line weights */
#define Z_LINE_WEIGHT1 0.25
#define Z_LINE_WEIGHT2 0.5
#define Z_LINE_WEIGHT3 0.75
#define Z_LINE_WEIGHT4 1
#define Z_LINE_WEIGHT5 1.25
#define Z_LINE_WEIGHT6 1.5
#define Z_LINE_WEIGHT7 1.75
#define Z_LINE_WEIGHT8 2.0

/* line type dimensions */
#define Z_LT_HIDDEN_ON 5.0
#define Z_LT_HIDDEN_OFF 5.0

#define Z_LT_CENTRE_ON 5.0
#define Z_LT_CENTRE_OFF 5.0
#define Z_LT_CENTRE_LONG 8.0

/* Font sizes */
#define Z_GRD_FONT_SZ 11
#define Z_GRD_FONT_STYLE "Century Gothic"
#define Z_GRD_GENERIC_SZ 4
#define Z_GRD_SPECIAL_SZ 7
#define Z_GRD_CUSTOMER_SZ 8
#define Z_GRD_WOZ_SZ 11

/* constructor helper */
#define ZCONSTRUCTOR(obj, type)				\
    if((obj) == NULL)					\
	{						\
	    (obj) = (type*) malloc(sizeof(type));	\
	    (obj)->_init_flg = ZELIA_CONSTRUCTED;	\
	}						\
    else						\
	(obj)->_init_flg = ZELIA_INITIALISED;		\
    if((obj) == NULL)					\
	return NULL

#define ZDESTRUCTOR_CHECK							\
	((obj->_int_flg == ZELIA_CONSTRUCTED)? 1 : 0)
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
#define ZCHECK_OBJ_PTR(obj)			\
    if((obj) == NULL)				\
	return NULL
#define ZCHECK_OBJ_DOUBLE(obj)					\
	if((obj)) == NULL)							\
	   return 0.0

#define ZELIA_LOG_MESSAGE(msg)			\
    fprintf(stdout, "%s\n", (msg))
#define ZELIA_LOG_MESSAGE_WITH_INT(msg, val)	\
    fprintf(stdout, "%s%i\n", (msg), (val))
#define ZELIA_LOG_MESSAGE_WITH_STR(msg, val)	\
    fprintf(stdout, "%s%s\n", (msg), (val))

/* object conversion macros */
#define Z_GENERIC(obj) (obj->super_cls)
#define Z_GENERICS(obj) (obj->super_cls)
#define Z_BASE(obj) ((zbase*) (obj)->child)
#define Z_TCELL(obj) ((ztcell*) Z_BASE(obj)->child)
#define Z_TCELLS(obj) ((ztcell*) (obj)->child)
#define Z_TROW(obj) ((ztrow*) Z_BASE(obj)->child)
#define Z_TROWS(obj) ((ztrows*) (obj)->child)
#define Z_TABLE(obj) ((ztable*) Z_BASE(obj)->child)
#endif /* __ZVAR__ */
