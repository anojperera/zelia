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
#define Z_DEFAULT_BORDER_SZ 5				/* default border */
#define Z_DEFAULT_GRID_SZ 5
#define Z_DEFAULT_GRID_SZ_WIDE 8
#define Z_DEFAULT_GRID_PITCH 45

#define Z_WOZ_MAIN_BRD_WIDTH 187.2			/* main border width */
#define Z_WOZ_MAIN_BRD_HEIGHT 77			/* main border height */
#define Z_WOZ_MAIN_BRD_BOTTOM 3				/* main border bottom clearance */

#define Z_WOZ_MAIN_DES_HEIGHT 18 			/* description height */
#define Z_WOZ_MAIN_CLT_HEIGHT 7.35			/* client height */
#define Z_WOZ_MAIN_PROJ_HEIGHT 6			/* project height */
#define Z_WOZ_MAIN_JOB_HEIGHT 6				/* job height */
#define Z_WOZ_MAIN_GEN_HEIGHT 6				/* general height */

#define Z_WOZ_MAIN_GEN_LEFT 52.5			/* left of checked and date column */
#define Z_WOZ_MAIN_PROJ_LEFT 89.5			/* left of projection column */
#define Z_WOZ_MAIN_REV_LEFT 16				/* left of border to rev */
#define Z_WOZ_MAIN_SHT_LEFT 35.5			/* sheet number column left */
#define Z_WOZ_MAIN_SHTMIN_LEFT 44
#define Z_WOZ_MAIN_MAIN_CLIENT_LEFT 145.5		/* client column left */

#define Z_WOZ_MAIN_PROJN_HEIGHT 12			/* projection column height */
#define Z_WOZ_MAIN_REVBOX_WIDTH 92			/* rev box width */
#define Z_WOZ_MAIN_REVBOX_HEIGHT 14 			/* rev box height */
#define Z_WOZ_MAIN_REVBOXMIN_WIDTH 10.5
#define Z_WOZ_MAIN_REVBOXMIN_HEIGHT 7.5

#define Z_WOZ_MAIN_DRWGBOX_WIDTH 45			/* drawing box of main rev box */
#define Z_WOZ_MAIN_SHTBOX_WIDTH 67.5			/* sheet number box */

#define Z_WOZ_MAIN_LOGO_WIDTH 46			/* logo width */
#define Z_WOZ_MAIN_LOGO_HEIGHT 39			/* logo height */

#define Z_BORDER_TXT_CLEAR 0.5				/* text clearance from top */
#define Z_BORDER_TXT_ATTRIB_HEIGHT 4			/* text height */
#define Z_BORDER_TXT_LEFT_ADJ 3				/* horizontal left adjustment */
#define Z_BORDER_TXT_LEFT_ADJ2 1
#define Z_BORDER_TXT_LEFT_ADJ3 6
#define Z_BORDER_TXT_LEFT_ADJ4 15
#define Z_BORDER_TXT_TOP_ADJ 1				/* vertical top adjustment */
#define Z_BORDER_TXT_TOP_ADJ1 4				/* vertical top adjustment */
#define Z_BORDER_TXT_TOP_ADJ2 5.5			/* vertical top adjustment */

#define Z_BORDER_TXT_TOP_ADJ3 0.1			/* vertical top adjustment */
#define Z_BORDER_TXT_TOP_ADJ4 1

#define Z_WOZ_MAIN_TITLE_TXT_HEIGHT 8			/* title text height */
#define Z_WOZ_MAIN_ATTRIB_VOFFSET 8			/* vertical offset */
#define Z_WOZ_MAIN_ATTRIB_HOFFSET 12 			/* horizontal offset */
#define Z_WOZ_MAIN_TITLE_CLEAR 2			/* clearance */
#define Z_WOZ_MAIN_ADRS_TITLE 6				/* address title */
#define Z_WOZ_MAIN_DRGNUM_SZ 6				/* drawing number text size */
#define Z_WOZ_MAIN_TATRBI_VOFFSET 8			/* special offset for
							   drawing number and
							   sheet number */
#define Z_WOZ_MAIN_TATRBI_HOFFSET 5			/* special horizontal offset */
#define Z_WOZ_MAIN_DWGNUM_HOFFSET 12	 		/* drawing number offset */
#define Z_DEFAULT_LINE_WIDTH 1				/* default line width */

#define Z_BORDER_MAT_BOX_WIDTH 95			/* material box width */
#define Z_BORDER_MAT_BOX_HEIGHT 8			/* material box height */

#define Z_BORDER_ATTRIB_MM_DIST 135			/* attrib header mm x distance */
#define Z_BORDER_ATTRIB_SCALE_DIST 234		/* attribute scale distance */
#define Z_BORDER_ATTRIB_BUR_DIST 282		/* attribute burr note distance */

#define Z_BORDER_ATTRIB_SCALE_DIST_A4_PORT 150	/* scale dist for A4 portrait
						   sheet */
#define Z_BORDER_ATTRIB_BUR_DIST_A4_PORT 190	/* debur note dist for A4 portrait
						   sheet */
/* projection constants */
#define Z_BORDER_PROJN_DIA_MAJ 8			/* projection major diameter */
#define Z_BORDER_PROJN_DIA_MIN 4			/* projection minor diameter */
#define Z_BORDER_PROJN_CYL_DEPTH 10			/* projection cylinder depth */

#define Z_BORDER_PROJN_DASH_ON 2.0			/* center line */
#define Z_BORDER_PROJN_DASH_OFF 0.5			/* center line */
#define Z_BORDER_PROJN_CLINE_VOFF 1


#define Z_MAX_ATTRIB 20

/* drawing attribute words *
 * attribute word describes drawing titles *
 * and header data */

#define Z_BORDER_TXT_DRG_NUMBER "DWG NO"		/* drawing number */
#define Z_BORDER_TXT_SHT_NUMBER "SHT"			/* sheet number */
#define Z_BORDER_TXT_REV_NUMBER "Rev"			/* revision number */
#define Z_BORDER_TXT_DIM_SPEC "UNLESS OTHERWISE SPECIFIED DIMENSIONS ARE IN MILIMETERS"
#define Z_BORDER_TXT_SCL_SPEC "DO NOT SCALE DRAWING"
#define Z_BORDER_TXT_FINISH_SPEC "DEBUR AND BREAK SHARP EDGES"
#define Z_BORDER_TXT_MAT_DES "MATERIAL: "

#define Z_BORDER_TXT_QTY_DES "QTY: "

#define Z_BORDER_TXT_TITLE_DES "TITLE"
#define Z_BORDER_TXT_CLIENT_DES "Client.:"
#define Z_BORDER_TXT_PROJECT_DES "Project.:"
#define Z_BORDER_TXT_JOB_DES "Job No.:"
#define Z_BORDER_TXT_CLIENTORDER_DES "Client's Order No.:"
#define Z_BORDER_TXT_DRAWN_DES "Drawn:"
#define Z_BORDER_TXT_CHKD_DES "Chk'd:"
#define Z_BORDER_TXT_APPV_DES "Approved:"
#define Z_BORDER_TXT_DATE_DES "Date:"
#define Z_BORDER_TXT_SCALE "Scale"


#define Z_BORDER_TXT_WOZ_DES "Wozair Ltd"
#define Z_BORDER_TXT_WOZ_ADRS1 "5 & 6 Saracen Close"
#define Z_BORDER_TXT_WOZ_ADRS2 "Gillingham Business Park"
#define Z_BORDER_TXT_WOZ_ADRS3 "Gillingham"
#define Z_BORDER_TXT_WOZ_ADRS4 "Kent ME8 0QN"
#define Z_BORDER_TXT_WOZ_ADRS5 "United Kingdom"
#define Z_BORDER_TXT_WOZ_ADRS6 "Tel: +44 (0) 1634263344"
#define Z_BORDER_TXT_WOZ_ADRS7 "Fax: +44 (0) 1634263377"
#define Z_BORDER_TXT_WOZ_EML "e-mail: hvac@wozair.com"
#define Z_BORDER_TXT_WOZ_COPY "This drawing is the copy right of\nWozair Ltd"

/* Font sizes */
#define Z_GRD_FONT_SZ 11
#define Z_GRD_FONT_STYLE "Century Gothic"
#define Z_GRD_GENERIC_SZ 4
#define Z_GRD_SPECIAL_SZ 7
#define Z_GRD_CUSTOMER_SZ 8
#define Z_GRD_WOZ_SZ 11


#define Z_GRD_WOZ_ADRS_SZ 5.5
#define Z_GRD_TITLE_SZ 16
#define Z_GRD_SPECIAL2_SZ 12
#define Z_GRD_MAT_SZ 8

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

/* Note default indent */
#define Z_NOTE_INDENT 10.0
#define Z_NOTE_LINE_HEIGHT 8.0
#define Z_NOTE_FONT_STYLE "Century Gothic 10"
/* Object conversion macro */
#define Z_SHEET(obj) ((zSheet*) obj->z_child)
#define Z_BASE(obj) ((zBase*) obj->z_child)
#define Z_TERMINAL(obj) ((zTerminal*) Z_BASE(obj)->z_child)
#define Z_TERMINALS(obj) ((zTerminals*) obj->z_child)
#define Z_JB(obj) ((zJB*) Z_BASE(obj)->z_child)
#define Z_GLAND(obj) ((zGland*) Z_BASE(obj)->z_child)
#define Z_GLANDS(obj) ((zGlands*) obj->z_child)
#define Z_ARROW(obj) ((zArrow*) Z_BASE(obj)->z_child)
#define Z_LEADER(obj) ((zLeader*) Z_ARROW(obj)->z_child)
#define Z_TCELL(obj) ((zTCell*) Z_BASE(obj)->z_child)
#define Z_TCELLS(obj) ((zTCells*) obj->z_child)
#define Z_TROW(obj) ((zTRow*) Z_BASE(obj)->z_child)
#define Z_TROWS(obj) ((zTRows*) obj->z_child)
#define Z_TABLE(obj) ((zTable*) Z_BASE(obj)->z_child)
#define Z_NOTE(obj) ((zNote*) Z_BASE(obj)->z_child)
#define Z_NOTES(obj) ((zNotes*) obj->z_child)
#define Z_DFRAME(obj) ((zDFrame*) Z_BASE(obj)->z_child)
#define Z_DSIDE_FRAME(obj) ((zDSideFrm*) Z_DFRAME(obj)->z_child)
#define Z_DTB_FRAME(obj) ((zDTBFrm*) Z_DFRAME(obj)->z_child)

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

/* error messages */
#define Z_ERROR_PANGO_LAYOUT "Unable to create pango layout"
#define Z_ERROR_LOGO_FAILED "Unable to load logo, check path"

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

/* convert milimeters to points */
static inline double ConvToPoints(double* mm)
{
	return (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * (*mm);
}

#define CONV_TO_POINTS(val) \
    (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * val

#define CONV_TO_RADIANS(val) \
    (M_PI * val / 180.0)

#define CONV_TO_PANGO(val) \
    (Z_POINT_TO_INCHES / Z_MM_TO_INCHES) * val * PANGO_SCALE

/* rounds the number */
static inline double Round(double val, unsigned int places)
{
	double off = pow(10, places);
	
	double x = val * off;
	double b = 0;
	double i_part = 0;
	
	
	if(modf(x, &i_part) >= 0.5)
		b = (x>=i_part? ceil(x) : floor(x));
	else
		b = (x<i_part? ceil(x) : floor(x));

	return b/off;

}

/* copy string to buffer */
static inline int zcCopy(char** buff, const char* val)
{
    size_t sz;
    if(val)
	{
	    /* obtain string length */
	    sz = strlen(val);

	    /* return 0 */
	    if(!sz)
		return 0;
			
	    /* create buffer */
	    *buff = (char*) malloc(sizeof(char) * (sz+1));
	    if(*buff != NULL)
		strcpy(*buff, val);

	    return sz;
	}

    return 0;
}

/* size of buffer */
static inline int zBuffLen(int arr_sz, char** buff)
{
    int i;
    int len = 0;
    if(!arr_sz && !buff)
	return 0;

    for(i=0; i<arr_sz; i++)
	{
	    len += strlen(buff[i]) + 1;
	}
	
    return len;
}

/* copy to buffer */
static inline int zCopyToBuff(char** ch, char** buff
			      , int arr_sz)
{
    int i;    
    if(!ch && !buff && arr_sz == 0)
	return 0;

    for(i=0; i<arr_sz; i++)
	{
	    if(i==0)
		{
		    if(buff[i] != NULL)
			strcpy(*ch, buff[i]);
		    else
			strcpy(*ch, "0");
		}
	    else
		{
		    strcat(*ch, "\n");

		    if(buff[i] != NULL)
			strcat(*ch, buff[i]);
		    else
			strcpy(*ch, "0");
		}
	}

    return strlen(*ch);
}
#endif /* __ZVAR__ */
