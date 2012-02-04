/* main test file
   Sun Oct 17 13:07:08 BST 2010 */
#include <mcheck.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cairo/cairo.h>

#include "zDevice.h"
#include "zGeneric.h"
#include "zGenerics.h"
#include "zSheet.h"
#include "zBase.h"
#include "zTerminal.h"
#include "zTerminals.h"
#include "zJB.h"
#include "zGlands.h"

int main(int argc, char** argv)
{
    const int TXT_SZ = 20;
    zDevice dev;		/* device object */
    zGeneric* jb;		/* JB object */
    zGenerics* glands;		/* Gland */
    
    zDevice_New2(zFormatPDF,
		 zSheetA3_Landscape,
		 "test.pdf",
		 0,
		 &dev);

    /* create sheet */
    zGeneric* sht = zSheet_New(NULL);
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


    jb = zJB_New(NULL,
		 &dev,
		 150.0,			/* x coordinate */
		 50.0,			/* y coordiante */
		 100.0,			/* width */
		 120.0,			/* height */
		 90.0,			/* depth */
		 0.0);			/* rotation angle */

    /* set JB radius */
    zJB_Set_Fillet_Radius(Z_JB(jb), 10.0);
    
    /* create terminals */
    zJB_Add_Terminals(Z_JB(jb),
		      6,
		      10.0,
		      40.0,
		      "1-3,5-6");

    /* Create a cable gland array */
    glands = zGlands_New(NULL);
    zGenerics_Set_Device(glands, &dev);
    /* Add glands */
    zGlands_Add(Z_GLANDS(glands),
		NULL,
		50.0,
		50.0,
		zM20,
		1);

    zGlands_Add(Z_GLANDS(glands),
		NULL,
		50.0,
		100.0,
		zM20,
		1);

    
    /* create border */
    zGeneric_Draw(sht);
    zGeneric_Draw(jb);
    zGenerics_Draw(glands);
    
    /* delete objects */
    zSheet_Delete(Z_SHEET(sht));
    zJB_Delete(Z_JB(jb));
    zGlands_Delete(Z_GLANDS(glands));
    
    zDevice_Delete(&dev);

    if(attrib->z_title)
	free(attrib->z_title);
    
    if(attrib->z_client)
	free(attrib->z_client);

    if(attrib->z_site)
	free(attrib->z_site);

    attrib->z_title = NULL;
    attrib->z_client = NULL;
    attrib->z_site = NULL;

    if(attrib)
	free(attrib);
    return 0;
}
