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
	size_t z_obj_sz;			/* object size */
    };
		
    struct _zBrd_Attrib
    {
	char z_dwg_num[16];			/* drawing number */
	char z_sht_num[4];			/* sheet number */
	char z_rev_num[4];			/* revision number */
	char z_scale[8];			/* scale */
	char z_title[128];			/* title */
	char z_client[128];			/* client name */
	char z_site[128];			/* site / project*/
	char z_jobnum[16];			/* job */
	char z_ordnum[128];			/* order number */
	char z_drawn[8];			/* drawn */
	char z_drawn_date[8];			/* drawn date */
	char z_chked[8];			/* checked */
	char z_chk_date[8];			/* checked date */
	char z_app[8];				/* approved by */
	char z_app_date[8];			/* approved date */
	char z_material[32];			/* material */
	char z_qty[8];				/* quantity */
    };

    /* constructor and destructor */
    zGeneric* zSheet_New(zSheet* obj);
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
