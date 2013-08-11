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

#define Z_SHEET_PATH "/home/pyrus/Prog/C++/zelia/resources/wozair_border_a3.svg"
#define Z_DEV_FILE_NAME "temp.svg"

int main(int argc, char** argv)
{
    zDevice dev;		/* device object */
    zGeneric* sht;		/* Sheet class */

    /* create a new device */
    zDevice_New2(zFormatSVG, 			       /* output format */
		 zSheetA3_Landscape,
		 Z_DEV_FILE_NAME,
		 1,
		 &dev);

    /* create sheet */
    sht = zSheet_New(NULL);

    zSheet_Set_Template_Path(Z_SHEET(sht), Z_SHEET_PATH);
    zGeneric_Set_Device(sht, &dev);
    zGeneric_Set_Default_Dev_Context(sht);

    zGeneric_Set_Defauts(sht);

    zGeneric_Draw(sht);
    return 0;
}
