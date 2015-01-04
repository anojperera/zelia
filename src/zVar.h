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

typedef enum {
    /* item classes */
    zunknown_type,
    zgeneric_type,
    zfile_type,
    zfile_attrib_type,
    zbase_type,
    zgland_type,
    zjb_type,
    zterminal_type,
    znote_type,
    ztcell_type,
    ztrow_type,
    ztable_type,
    zarrow_type,
    zleader_type,
    zlabel_type,

    /* collection classes */
    zgenerics_type,
    zglands_type,
    znotes_type,
    zterminals_type,
    ztrows_type,
    ztcells_type
} zelia_object_types;

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

/* Terminal types */
typedef enum {
    zSAK2pt5,
    zEK4
} zTerminalType;


/* Gable Gland Sizes */
typedef enum {
    zM16,
    zM20,
    zM25
} zGlandSize;

/* Arrow head types */
typedef enum {
    zArrow1,
    zArrow2,
    zArrow3
} zArrowTypes;

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
#define ZELIA_TERINALS_ERROR -9
#define ZELIA_JB_ERROR -10
#define ZELIA_NOTE_ERROR -11
#define ZELIA_NOTES_ERROR -12
#define ZELIA_UNAUTHORISE -13
#define ZELIA_PARSER_ERROR -14

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

#define Z_NOTE_INDENT 10.0
#define Z_NOTE_LINE_HEIGHT 8.0
#define Z_NOTE_FONT_STYLE "Century Gothic 10"

/* Diameters */
#define Z_M16_GLAND 16.0
#define Z_M20_GLAND 20.0
#define Z_M25_GLAND 25.0

/* Arrow angles */
#define Z_ARROW_ANG1 45.0
#define Z_ARROW_ANG2 30.0

#define Z_ARROW_HEIGHT1 3.0
#define Z_ARROW_HEIGHT2 5.0
#define Z_ARROW_HEIGHT3 8.0

#define Z_EARTH_TERMINALS_ANNOT_DEFAULT "EK"
#define Z_EARTH_TERMINALS_EXTRA_WDITH 2.0
#define Z_EARTH_TERMINALS_EXTRA_HEIGHT 3.0

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

#define ZDESTRUCTOR_CHECK				\
    ((obj->_init_flg == ZELIA_CONSTRUCTED)? 1 : 0)
#define ZCONV_TO_POINTS(val)			\
    (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * val
#define ZCONV_TO_MM(val)			\
    (Z_MM_TO_INCHES / Z_POINT_TO_INCHES) * val

#define ZCONV_TO_RADIANS(val)			\
    (M_PI * val / 180.0)

#define ZCONV_TO_PANGO(val)					\
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
#define ZCHECK_OBJ_DOUBLE(obj)			\
    if((obj) == NULL)				\
	return 0.0

#define ZELIA_LOG_MESSAGE(msg)			\
    fprintf(stdout, "%s - %s line %i\n", (msg), __FILE__, __LINE__)
#define ZELIA_LOG_MESSAGE_WITH_INT(msg, val)	\
    fprintf(stdout, "%s%i - %s line %i\n", (msg), (val), __FILE__, __LINE__)
#define ZELIA_LOG_MESSAGE_WITH_STR(msg, val)	\
    fprintf(stdout, "%s%s - %s line %i\n", (msg), (val), __FILE__, __LINE__)

/* object conversion macros */
#define Z_GENERIC(obj) ((obj)? (obj)->super_cls : NULL)
#define Z_GENERICS(obj) ((obj)? (obj)->super_cls : NULL)
#define Z_BASE(obj) ((obj)? (zbase*) (obj)->child : NULL)
#define Z_TCELL(obj) ((obj)? (ztcell*) Z_BASE(obj)->child : NULL)
#define Z_TCELLS(obj) ((obj)? (ztcells*) (obj)->child : NULL)
#define Z_TROW(obj) ((obj)? (ztrow*) Z_BASE(obj)->child: NULL)
#define Z_TROWS(obj) ((obj)? (ztrows*) (obj)->child : NULL)
#define Z_TABLE(obj) ((obj)? (ztable*) Z_BASE(obj)->child : NULL)
#define Z_TERMINAL(obj) ((obj)? (zterminal*) Z_BASE(obj)->child : NULL)
#define Z_TERMINALS(obj) ((obj)? (zterminals*) (obj)->child : NULL)
#define Z_GLAND(obj) ((obj)? (zgland*) Z_BASE(obj)->child : NULL)
#define Z_GLANDS(obj) ((obj)? (zglands*) (obj)->child : NULL)
#define Z_JB(obj) ((obj)? (zjb*) Z_BASE(obj)->child : NULL)
#define Z_NOTE(obj) ((obj)? (znote*) Z_BASE(obj)->child : NULL)
#define Z_NOTES(obj) ((obj)? (znotes*) (obj)->child : NULL)
#define Z_ARROW(obj) ((obj)? (zarrow*) Z_BASE(obj)->child : NULL)
#define Z_LEADER(obj) ((obj)? (zleader*) Z_ARROW(obj)->child : NULL)
#define Z_LABEL(obj) ((obj)? (zlabel*) Z_BASE(obj)->child : NULL)

#endif /* __ZVAR__ */
