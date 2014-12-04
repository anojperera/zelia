/* Temporary function to opern and draw a box and save */

#include <stdlib.h>
#include <stdio.h>
#include "zVar.h"
#include "zfile.h"
#include "zfile_attrib.h"

#include "zdevice.h"
#include "zgeneric.h"
#include "zgenerics.h"
#include "zbase.h"
#include "zterminal.h"
#include "zterminals.h"
#include "zjb.h"
#include "zglands.h"
#include "ztable.h"
#include "znote.h"
#include "znotes.h"


#define Z_FILE_PATH "/home/pyrus/Prog/C++/zelia/resources/wozair_border_a4.svg"
#define Z_FILE_SAVE_PATH "/home/pyrus/temp.svg"
#define Z_FILE_XML_ATTRIB_PATH "/home/pyrus/Prog/C++/zelia/src/attrib.xml"

#define Z_FILE_ATTRIB_ARRAY			\
    {						\
	{ "drawing_no", "C123456"},		\
	{ "sheet_no", "2"},			\
	{NULL, NULL}				\
    }

int create_drawing(int argc, char** argv);

int main(int argc, char** argv)
{
    zfile _file;
    zfile_attrib _attrib;

    int stat = 0;
    
    if(zfile_new(&_file) == NULL)
	return -1;

    /* toggle the force overwrite mode */
    zfile_toggle_overwrite(&_file);
    
    stat = zfile_create_file_from_template(&_file, Z_FILE_PATH, Z_FILE_SAVE_PATH);
    if(stat != ZELIA_OK)
	ZELIA_LOG_MESSAGE("errors occured while creating a new file");

    zfile_attrib_new(&_attrib, zfile_get_xmldoc_ptr(&_file));
    zfile_attrib_set_ext_obj(&_attrib, (void*) &_file);

    zfile_attrib_add_complete_hook(&_attrib, zfile_get_callback_hook(&_file));
    zfile_attrib_parse_attrib_xml(&_attrib, Z_FILE_XML_ATTRIB_PATH);
    zfile_attrib_set_field_array(&_attrib, NULL, 0);

    create_drawing(argc, argv);

    /* clean up */
    zfile_attrib_delete(&_attrib);
    zfile_delete(&_file);
    return 0;
}





int create_drawing(int argc, char** argv)
{
    zdevice dev;		/* device object */

    zgeneric* table;		/* table */
    zgenerics* notes;		/* note */


    zdevice_new2(zSheetA3_Landscape,
		 0,
		 &dev);


    table = ztable_new(NULL);
    zgeneric_set_device(table, &dev);

    zbase_set_base_coords(Z_BASE(table), 40.0, 120.0);
    zbase_set_width_and_height(Z_BASE(table), 160.0, 30.0);
    ztable_set_rows_and_cols(Z_TABLE(table), 3, 4);
    ztable_set_column_width(Z_TABLE(table), 0, 60.0);
    ztable_set_column_width(Z_TABLE(table), 2, 60.0);
    ztable_set_content(Z_TABLE(table), 0, 1, "DAMPERS");

    notes = znotes_new(NULL, &dev, 260.0, 130.0, 150.0);
    znotes_set_title(Z_NOTES(notes), "DAMPER OPERATION");
    znotes_add(Z_NOTES(notes), "FRAMES ARE CONTINUOUSLY WELDED CONSTRUCTION.");
    znotes_add(Z_NOTES(notes), "DAMPER BLADES TO BE HOT DIP GALVANISED.");
    znotes_add(Z_NOTES(notes), "BLADES ARE WELDED TO SHAFTS.");

    /* zGeneric_Draw(table); */
    zgenerics_draw(notes);
    zgeneric_draw(table);

    /* delete objects */
    ztable_delete(Z_TABLE(table));
    znotes_delete(Z_NOTES(notes));

    
    zdevice_delete(&dev);

    return 0;
}
