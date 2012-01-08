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
	zGeneric* z_sgeneric;			/* inherited generic class */

	char* z_slogo_path;			/* logo path */
	zBrd_Attrib** z_sbrd_attrib;		/* border attributes */
	int z_sgrid_flg;			/* grid flag */
	int z_ssafe_flg;			/* flag to indicate
						   border created */
	
	double z_x_attrib[Z_MAX_ATTRIB];	/* maximum number of
						   attributes */
	double z_y_attrib[Z_MAX_ATTRIB];		
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
    zGeneric* zSheet_Create();
    void zSheet_Delete(zSheet** obj);

    /* set and get device */
    /* void zSheet_Set_Device(zSheet* obj, zDevice** var); */
    /* zDevice* zSheet_Get_Device(zSheet* obj); */

    /* set and get border attributes */
    void zSheet_Set_Attributes(zSheet* obj,
			       zBrd_Attrib** var);
    zBrd_Attrib* zSheet_Get_Attributes(zSheet* obj);

    /* set and get Grid Flag */
    void zSheet_Set_GridFlag(zSheet* obj, int var);
    int zSheet_Get_GridFlag(zSheet* obj);

    /* set and get logo path */
    void zSheet_Set_LogoPath(zSheet* obj, const char* var);
    char* zSheet_Get_LogoPath(zSheet* obj);
	
    /* creates border */
    void zSheet_Create_Border(zSheet* obj);
    
    /* private functions */
    inline int zsheet_draw_oborder(zSheet** obj);
    inline int zsheet_draw_grid(zSheet** obj);
    inline int zsheet_draw_top_revbox(zSheet** obj);
    inline int zsheet_draw_decal(zSheet** obj);

    inline int zsheet_draw_matbox(zSheet** obj);
    inline int zsheet_draw_projn(zSheet** obj);

    /* add drawing header details */
    inline int zsheet_add_attrib_headers(zSheet** obj);

    /* paints the logo into current context */
    inline int zsheet_paint_logo(zSheet** obj);

    /* constructs wozair address from
       constants */
    inline char* zsheet_wozair_address();

    /* add drawing attributes */
    inline int zsheet_add_attribs(zSheet** obj);
    

#ifdef __cplusplus
}
#endif

#endif	/* __ZSHEET__ */
