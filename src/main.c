/* main test file
   Sun Oct 17 13:07:08 BST 2010 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cairo/cairo.h>

#include "zDevice.h"
#include "zGeneric.h"
#include "zBase.h"
#include "zTerminal.h"
#include "zSheet.h"

int main(int argc, char** argv)
{
    const int TXT_SZ = 20;
    zDevice dev;		/* device object */
    
    zDevice_New2(zFormatPDF,
		 zSheetA3_Landscape,
		 "test.pdf",
		 0,
		 &dev);

    /* create sheet */
    zGeneric* sht = zSheet_New(NULL);
    zGeneric* term = zTerminal_New(NULL);
    /* check for NULL pointer */
    if(sht == NULL)
	{
	    zDevice_Delete(&dev);
	    return 1;
	}

    /* create drawing attributes */
    zBrd_Attrib* attrib = (zBrd_Attrib*)
	malloc(sizeof(zBrd_Attrib));

    attrib->z_dwg_num = NULL;
    attrib->z_sht_num = NULL;
    attrib->z_rev_num = NULL;
    attrib->z_scale = NULL;

    attrib->z_title = (char*) malloc(sizeof(char) * TXT_SZ);
    strcpy(attrib->z_title, "TEMP");

    attrib->z_client = (char*) malloc(sizeof(char) * TXT_SZ);
    strcpy(attrib->z_client, "HI-PRES");
	
    attrib->z_site = (char*) malloc(sizeof(char) * TXT_SZ);
    strcpy(attrib->z_site, "Over load");
	
    attrib->z_jobnum = NULL;
    attrib->z_ordnum = NULL;
    attrib->z_drawn = NULL;
    attrib->z_drawn_date = NULL;
    attrib->z_chked = NULL;
    attrib->z_chk_date = NULL;
    attrib->z_app = NULL;
    attrib->z_app_date = NULL;
    attrib->z_material = NULL;
    attrib->z_qty = NULL;
    /*************************************/

    /* set device */
    zGeneric_Set_Device(sht, &dev);
    zGeneric_Create_Dev_Context(sht);
    zGeneric_Set_LineWeight(sht, zLWeight1);
    zGeneric_Set_Defauts(sht);
    zSheet_Set_GridFlag(Z_SHEET(sht), 1);
    zSheet_Set_LogoPath(Z_SHEET(sht),
			"../resources/logo/wozair_logo2.png");

    /* set attributes */
    zSheet_Set_Attributes(Z_SHEET(sht), attrib);

    /* Set base coordinates */
    zGeneric_Set_Device(term, &dev);
    /* zGeneric_Create_Dev_Context(term); */
    zGeneric_Set_Default_Dev_Context(term);
    zBase_Set_Base_Coords(Z_BASE(term), 100.0, 40.0);
    zBase_Set_Width_and_Height(Z_BASE(term), 20.0, 50.0);

    /* create border */
    zGeneric_Draw(sht);
    zGeneric_Draw(term);
    
    /* delete objects */
    zSheet_Delete(Z_SHEET(sht));
    zTerminal_Delete(Z_TERMINAL(term));
    zDevice_Delete(&dev);

    if(attrib)
	free(attrib);
    return 0;
}
