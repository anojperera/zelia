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

/* Font sizes */
#define Z_GRD_FONT_SZ 11
#define Z_GRD_FONT_STYLE "Century Gothic"
#define Z_GRD_GENERIC_SZ 4
#define Z_GRD_SPECIAL_SZ 7
#define Z_GRD_CUSTOMER_SZ 8
#define Z_GRD_WOZ_SZ 11

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

/* Table font style */
#define Z_TABLE_FONT_STYLE "Century Gothic 10"

/* Note default indent */
#define Z_NOTE_INDENT 10.0
#define Z_NOTE_LINE_HEIGHT 8.0
#define Z_NOTE_FONT_STYLE "Century Gothic 10"

/* Object checking macro */
#define Z_CHECK_OBJ(obj) \
    if(!obj) \
	return 1

#define Z_CHECK_OBJ_PTR(obj) \
    if(!obj) \
	return NULL

#define Z_CHECK_OBJ_DOUBLE(obj) \
    if(!obj) \
	return 0.0

#define Z_CHECK_OBJ_VOID(obj) \
    if(!obj) \
	return

/* constructor helper */
#define Z_CONSTRUCTOR_HELPER(obj, ztype) \
    if(obj == NULL) \
	{ \
    obj = (ztype*) malloc(sizeof(ztype)); \
    Z_CHECK_OBJ_PTR(obj); \
    obj->z_int_flg = 1; \
	} \
    else \
	obj->z_int_flg = 0

/* output format */
typedef enum {
    zFormatPDF,
    zFormatSVG,
    zFormatPNG,
    zFormatDXF
} zOutputFormat;


/* paper sizes */
typedef enum {
    zSheetA3_Landscape,
    zSheetA4_Portrait
} zSheets;

/* line types */
typedef enum {
    zLTContinuous,
    zLTCenter,
    zLTHidden
} zLineTypes;

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

typedef enum {
    zLBlack,
    zLRed,
    zLBlue,
    zLYellow,
    zLCyan
} zLineColour;

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

/* Blade Types */
typedef enum {
    zBlade_ISO,
    zBlade_NISO,
    zBlade_LFD,
    zBlade_TOTAL,
    zBlade_BRD,
    zBlade_PRD
} zBladeType;

/* Blade assembly types */
typedef enum {
    zBladeISO_AA,
    zBladeISO_AB,
    zBladeISO_BB
} zBladeISO_Type;    

/* Bearing types */
typedef enum {
    zBearingCutdown,
    zBearingBlind,
    zBearingRemoval
} zBearingTypes;


#define CONV_TO_POINTS(val) \
    (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * val

#define CONV_TO_RADIANS(val) \
    (M_PI * val / 180.0)

#define CONV_TO_PANGO(val) \
    (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * val * PANGO_SCALE

#endif /* __ZVAR__ */
