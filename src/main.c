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
#include "zNote.h"
#include "zNotes.h"
#include "zDFrame.h"
#include "zDSideFrm.h"
#include "zDTBFrm.h"

int main(int argc, char** argv)
{
    zDevice dev;		/* device object */
    zGeneric* sht;
    zGeneric* table;		/* table */
    zGeneric* dfrm;		/* drive frame */
    zGeneric* tbfrm;		/* top frame */
    zGenerics* notes;		/* note */
    
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

    table = zTable_New(NULL);
    zGeneric_Set_Device(table, &dev);

    zBase_Set_Base_Coords(Z_BASE(table), 40.0, 120.0);
    zBase_Set_Width_and_Height(Z_BASE(table), 160.0, 30.0);
    zTable_Set_Rows_and_Cols(Z_TABLE(table), 3, 4);
    zTable_Set_Column_Width(Z_TABLE(table), 0, 60.0);
    zTable_Set_Column_Width(Z_TABLE(table), 2, 60.0);
    zTable_Set_Content(Z_TABLE(table), 0, 1, "DAMPERS");

    notes = zNotes_New(NULL, &dev, 40.0, 40.0, 200.0);
    zNotes_Set_Title(Z_NOTES(notes), "DAMPER OPERATION");
    zNotes_Add(Z_NOTES(notes), "FRAMES ARE CONTINUOUSLY WELDED CONSTRUCTION.");
    zNotes_Add(Z_NOTES(notes), "DAMPER BLADES TO BE HOT DIP GALVANISED.");
    zNotes_Add(Z_NOTES(notes), "BLADES ARE WELDED TO SHAFTS.");

    dfrm = zDSideFrm_New(NULL);
    zGeneric_Set_Device(dfrm, &dev);
    zGeneric_Set_Default_Dev_Context(dfrm);
    zBase_Set_Base_Coords(Z_BASE(dfrm), 260.0, 40.0);
    zBase_Set_Width_and_Height(Z_BASE(dfrm), 10.0, 100.0);
    zBase_Set_Thickness(Z_BASE(dfrm), 2.0);
    zDFrame_Set_Return_Lip_Flg(Z_DFRAME(dfrm), 1);

    tbfrm = zDTBFrm_New(NULL);
    zGeneric_Set_Device(tbfrm, &dev);
    zGeneric_Set_Default_Dev_Context(tbfrm);
    zBase_Set_Base_Coords(Z_BASE(tbfrm), 270.0, 40.0);
    zBase_Set_Width_and_Height(Z_BASE(tbfrm), 100.0, 10.0);

    /* create border */
    zGeneric_Draw(sht);
    zGeneric_Draw(dfrm);
    zGeneric_Draw(table);
    zGeneric_Draw(tbfrm);
    zGenerics_Draw(notes);
    
    /* delete objects */
    zSheet_Delete(Z_SHEET(sht));
    zTable_Delete(Z_TABLE(table));
    zNotes_Delete(Z_NOTES(notes));
    zDSideFrm_Delete(Z_DSIDE_FRAME(dfrm));
    zDTBFrm_Delete(Z_DTB_FRAME(tbfrm));
    zDevice_Delete(&dev);

    if(attrib)
	free(attrib);
    attrib = NULL;
    return 0;
}
