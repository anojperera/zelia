/* main test file
   Sun Oct 17 13:07:08 BST 2010 */
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
#include "zArrow.h"
#include "zLeader.h"
#include "zTable.h"

int main(int argc, char** argv)
{
    zDevice dev;		/* device object */
    zGeneric* jb;		/* JB object */
    zGenerics* glands;		/* Gland */
    zGeneric* sht;
    zGeneric* leader;		/* leader */
    zGeneric* table;		/* table */
    
    zBrd_Attrib* attrib;
    zDevice_New2(zFormatPDF,
		 zSheetA3_Landscape,
		 "test.pdf",
		 0,
		 &dev);

    /* create sheet */
    sht = zSheet_New(NULL);
    /* check for NULL pointer */
    if(sht == NULL)
	{
	    zDevice_Delete(&dev);
	    return 1;
	}

    /* create drawing attributes */
    attrib = (zBrd_Attrib*)
	malloc(sizeof(zBrd_Attrib));

    attrib->z_dwg_num[0] = '\0';
    attrib->z_sht_num[0] = '\0';
    attrib->z_rev_num[0] = '\0';
    attrib->z_scale[0] = '\0';

    strcpy(attrib->z_title, "TEMP");
    strcpy(attrib->z_client, "HI-PRES");
    strcpy(attrib->z_site, "Over load");
	
    attrib->z_jobnum[0] = '\0';
    attrib->z_ordnum[0] = '\0';
    attrib->z_drawn[0] = '\0';
    attrib->z_drawn_date[0] = '\0';
    attrib->z_chked[0] = '\0';
    attrib->z_chk_date[0] = '\0';
    attrib->z_app[0] = '\0';
    attrib->z_app_date[0] = '\0';
    attrib->z_material[0] = '\0';
    attrib->z_qty[0] = '\0';
    /*************************************/

    /* set device */
    zGeneric_Set_Device(sht, &dev);
    zGeneric_Create_Dev_Context(sht);
    zGeneric_Set_LineWeight(sht, zLWeight1);
    zGeneric_Set_Defauts(sht);
    zSheet_Set_GridFlag(Z_SHEET(sht), 1);
    /* zSheet_Set_LogoPath(Z_SHEET(sht), */
    /* 			"../resources/logo/wozair_logo2.png"); */

    /* set attributes */
    zSheet_Set_Attributes(Z_SHEET(sht), attrib);


    jb = zJB_New(NULL,
		 &dev,
		 250.0,			/* x coordinate */
		 40.0,			/* y coordiante */
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

    zJB_Add_Glands(Z_JB(jb),
		   30,
		   30,
		   zM20,
		   1);

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

    /* Leader */
    leader = zLeader_New(NULL,			/* object pointer */
			 &dev,			/* device object */
			 50.0,			/* x */
			 150.0,			/* y */
			 40.0,			/* length 1 */
			 20.0,			/* length 2 */
			 80.0,			/* angle */
			 NULL);			/* description */
    zArrow_Set_Arrow_Type(Z_ARROW(leader), zArrow3);
    zLeader_Set_Description(Z_LEADER(leader), "TEST");
    table = zTable_New(NULL);
    zGeneric_Set_Device(table, &dev);

    zBase_Set_Base_Coords(Z_BASE(table), 40.0, 120.0);
    zBase_Set_Width_and_Height(Z_BASE(table), 60.0, 10.0);
    zTable_Set_Rows_and_Cols(Z_TABLE(table), 1, 2);
    zTable_Set_Column_Width(Z_TABLE(table), 1, 60.0);
    zTable_Set_Content(Z_TABLE(table), 0, 1, "DAMPERS");

    /* create border */
    zGeneric_Draw(sht);
    zGeneric_Draw(table);
    zGeneric_Draw(jb);
    zGenerics_Draw(glands);
    zGeneric_Draw(leader);
    
    /* delete objects */
    zSheet_Delete(Z_SHEET(sht));
    zJB_Delete(Z_JB(jb));
    zGlands_Delete(Z_GLANDS(glands));
    zLeader_Delete(Z_LEADER(leader));
    zTable_Delete(Z_TABLE(table));
    zDevice_Delete(&dev);

    if(attrib)
	free(attrib);
    attrib = NULL;
    return 0;
}
