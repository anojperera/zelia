/* Declaration of sheet object
   Sat Oct 16 22:34:31 BST 2010 */

#ifndef __ZSHEET__
#define __ZSHEET__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cairo/cairo.h>
#include <pango-1.0/pango/pangocairo.h>

#include "zVar.h"
#include "zGeneric.h"
#include "zDevice.h"

/* forwar declaration of struct */
typedef struct _zBrd_Attrib zBrd_Attrib;
typedef struct _zSheet zSheet;

struct _zBrd_Attrib
{
    char z_dwg_num[Z_SHT_ATTRIB_SZ3];		/* drawing number */
    char z_sht_num[Z_SHT_ATTRIB_SZ1];		/* sheet number */
    char z_rev_num[Z_SHT_ATTRIB_SZ1];		/* revision number */
    char z_scale[Z_SHT_ATTRIB_SZ1];		/* scale */
    char z_title[Z_SHT_ATTRIB_SZ6];		/* title */
    char z_client[Z_SHT_ATTRIB_SZ6];		/* client name */
    char z_site[Z_SHT_ATTRIB_SZ6];		/* site / project*/
    char z_jobnum[Z_SHT_ATTRIB_SZ3];		/* job */
    char z_ordnum[Z_SHT_ATTRIB_SZ6];		/* order number */
    char z_drawn[Z_SHT_ATTRIB_SZ2];		/* drawn */
    char z_drawn_date[Z_SHT_ATTRIB_SZ2];	/* drawn date */
    char z_chked[Z_SHT_ATTRIB_SZ2];		/* checked */
    char z_chk_date[Z_SHT_ATTRIB_SZ2];		/* checked date */
    char z_app[Z_SHT_ATTRIB_SZ2];		/* approved by */
    char z_app_date[Z_SHT_ATTRIB_SZ2];		/* approved date */
    char z_material[Z_SHT_ATTRIB_SZ5];		/* material */
    char z_qty[Z_SHT_ATTRIB_SZ2];		/* quantity */
};

struct _zSheet
{
    zGeneric z_sgeneric;			/* inherited generic class */
    
    unsigned int z_int_flg;			/* internal flag */
    unsigned int z_attrib_init;			/* attribute initialised */
    char z_slogo_path[Z_SHEET_LOGO_PATH_SZ];	/* logo path */
    zBrd_Attrib z_sbrd_attrib;			/* border attributes */
    int z_sgrid_flg;				/* grid flag */
    int z_ssafe_flg;				/* flag to indicate
						 * border created */
	
    double z_x_attrib[Z_MAX_ATTRIB];		/* maximum number of
						 * attributes */
    double z_y_attrib[Z_MAX_ATTRIB];
    
    zgeneric_fptr z_draw_func;			/* draw function */
    void* z_child;				/* child pointer */
    const char* z_tmp_path;			/* set template path */        
    size_t z_obj_sz;				/* object size */
};

/* const int Z_MAX_ATTRIB = 20; */
		
#ifdef __cplusplus
extern "C" {
#endif
    
    /* constructor and destructor */
    zGeneric* zSheet_New(zSheet* obj);
    void zSheet_Delete(zSheet* obj);

    /* set and get border attributes */
    inline int zSheet_Set_Attributes(zSheet* obj, zBrd_Attrib* var);
    inline __attribute__ ((always_inline)) static zBrd_Attrib* zSheet_Get_Attributes(zSheet* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);
	return &obj->z_sbrd_attrib;
    }

    /* set and get Grid Flag */
    inline __attribute__ ((always_inline)) static int zSheet_Set_GridFlag(zSheet* obj, int var)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ(obj);
	obj->z_sgrid_flg = var;
	return 0;
    }
    
    inline __attribute__ ((always_inline)) static int zSheet_Get_GridFlag(zSheet* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ(obj);
	return obj->z_sgrid_flg;
    }

    /* set and get logo path */
    inline int __attribute__ ((always_inline)) static zSheet_Set_LogoPath(zSheet* obj, const char* var)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ(obj);
	Z_CHECK_OBJ(var);

	/* copy to buffer */
	strncpy(obj->z_slogo_path, var, Z_SHEET_LOGO_PATH_SZ-1);

	return 0;
    }

    /* get logo path */
    inline __attribute__ ((always_inline)) static char* zSheet_Get_LogoPath(zSheet* obj)
    {
	/* check for NULL pointer */
	Z_CHECK_OBJ_PTR(obj);
	return obj->z_slogo_path;
    }

    /* set template path */
    inline __attribute__ ((always_inline)) static int zSheet_Set_Template_Path(zSheet* obj, const char* path)
    {
	if(obj != NULL && path != NULL)
	    obj->z_tmp_path = path;
	return 0;
    }
    

    /* creates border */
    int zSheet_Create_Border(zSheet* obj);
#ifdef __cplusplus
}
#endif

#endif	/* __ZSHEET__ */
