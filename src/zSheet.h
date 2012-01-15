/* Declaration of sheet object
   Sat Oct 16 22:34:31 BST 2010 */

#ifndef __ZSHEET__
#define __ZSHEET__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include "zVar.h"
#include "zGeneric.h"
#include "zDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* forwar declaration of struct */
    typedef struct _zBrd_Attrib zBrd_Attrib;
    typedef struct _zSheet zSheet;

    /* const int Z_MAX_ATTRIB = 20; */

    struct _zSheet
    {
	zGeneric z_sgeneric;			/* inherited generic class */
	unsigned int z_int_flg;			/* internal flag */
	char z_slogo_path[256];			/* logo path */
	zBrd_Attrib* z_sbrd_attrib;		/* border attributes */
	int z_sgrid_flg;			/* grid flag */
	int z_ssafe_flg;			/* flag to indicate
						   border created */
	
	double z_x_attrib[Z_MAX_ATTRIB];	/* maximum number of
						   attributes */
	double z_y_attrib[Z_MAX_ATTRIB];
	zgeneric_fptr z_draw_func;		/* draw function */
	void* z_child;				/* child pointer */
    };
		
    struct _zBrd_Attrib
    {
	char* z_dwg_num;			/* drawing number */
	char* z_sht_num;			/* sheet number */
	char* z_rev_num;			/* revision number */
	char* z_scale;				/* scale */
	char* z_title;				/* title */
	char* z_client;				/* client name */
	char* z_site;				/* site / project*/
	char* z_jobnum;				/* job */
	char* z_ordnum;				/* order number */
	char* z_drawn;				/* drawn */
	char* z_drawn_date;			/* drawn date */
	char* z_chked;				/* checked */
	char* z_chk_date;			/* checked date */
	char* z_app;				/* approved by */
	char* z_app_date;			/* approved date */
	char* z_material;			/* material */
	char* z_qty;				/* quantity */
    };

    /* constructor and destructor */
    zGeneric* zSheet_Create(zSheet* obj);
    void zSheet_Delete(zSheet* obj);

    /* set and get device */
    /* void zSheet_Set_Device(zSheet* obj, zDevice** var); */
    /* zDevice* zSheet_Get_Device(zSheet* obj); */

    /* set and get border attributes */
    inline int zSheet_Set_Attributes(zSheet* obj,
			       zBrd_Attrib* var);
    inline zBrd_Attrib* zSheet_Get_Attributes(zSheet* obj);

    /* set and get Grid Flag */
    inline int zSheet_Set_GridFlag(zSheet* obj, int var);
    inline int zSheet_Get_GridFlag(zSheet* obj);

    /* set and get logo path */
    inline int zSheet_Set_LogoPath(zSheet* obj, const char* var);
    inline char* zSheet_Get_LogoPath(zSheet* obj);

    /* creates border */
    int zSheet_Create_Border(zSheet* obj);
#ifdef __cplusplus
}
#endif

#endif	/* __ZSHEET__ */
