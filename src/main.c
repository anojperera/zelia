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
#include "zgland.h"
#include "zglands.h"
#include "ztable.h"
#include "znote.h"
#include "znotes.h"


#define Z_FILE_PATH "/home/pyrus/Prog/C++/zelia/resources/wozair_border_a4.svg"
#define Z_FILE_SAVE_PATH "/home/pyrus/temp.svg"
#define Z_FILE_XML_ATTRIB_PATH "/home/pyrus/Prog/C++/zelia/resources/drawing.xml"

#define Z_FILE_ATTRIB_ARRAY			\
    {						\
	{ "drawing_no", "C123456"},		\
	{ "sheet_no", "2"},			\
	{NULL, NULL}				\
    }

int test(int argc, char** argv);
int create_drawing(zfile* obj);
int create_gland(int argc, char** argv);
int create_glands(int argc, char** argv);
int create_note(int argc, char** argv);
int create_jb(zfile* obj);

extern int zelia_parse_file(const char* xml_path);

int main(int argc, char** argv)
{
    zelia_parse_file(Z_FILE_XML_ATTRIB_PATH);
    /* test(argc, argv); */
    return 0;
}

int test(int argc, char** argv)
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

    /* create_gland(argc, argv); */
    /* create_glands(argc, argv); */
    /* create_drawing(&_file); */
    /* create_note(argc, argv);     */
    create_jb(&_file);

    /* clean up */
    zfile_attrib_delete(&_attrib);
    zfile_delete(&_file);

    return 0;
}

int create_drawing(zfile* obj)
{
    zdevice dev;		/* device object */

    zgeneric* table;		/* table */
    zgenerics* notes;		/* note */

    const char* _buff = NULL;

    zdevice_new2(zSheetA3_Landscape,
		 0,
		 &dev);


    table = ztable_new(NULL);
    zgeneric_set_device(table, &dev);
    zgeneric_set_default_dev_context(table);

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


    zgenerics_draw(notes);
    zgeneric_draw(table);

    /* get buffer */
    _buff = zdevice_get_temp_buff(&dev);

    /* add to the file object */
    zfile_parse_and_insert_elements(obj, _buff);

    /* delete objects */
    ztable_delete(Z_TABLE(table));
    znotes_delete(Z_NOTES(notes));


    zdevice_delete(&dev);

    return 0;
}

/* creates a gland for testing */
int create_gland(int argc, char** argv)
{
    zdevice dev;
    zgeneric* gland =  NULL;

    zdevice_new2(zSheetA3_Landscape,
		 0,
		 &dev);


    gland = zgland_new(NULL,
		       &dev,
		       50.0,
		       60.0,
		       zM20);

    zgland_draw(Z_GLAND(gland));

    zgland_delete(Z_GLAND(gland));
    zdevice_delete(&dev);

    return 0;
}

/* creates a gland for testing */
int create_glands(int argc, char** argv)
{
    zdevice dev;
    zgenerics* glands =  NULL;

    zdevice_new2(zSheetA3_Landscape,
		 0,
		 &dev);


    glands = zglands_new(NULL);

    /* add glands */
    zglands_add(Z_GLANDS(glands),
		&dev,
		60.0,
		50.0,
		zM20,
		1);

    zglands_add(Z_GLANDS(glands),
		&dev,
		100.0,
		50.0,
		zM20,
		1);

    zglands_add(Z_GLANDS(glands),
		&dev,
		130.0,
		50.0,
		zM20,
		1);

    zglands_add(Z_GLANDS(glands),
		&dev,
		130.0,
		100.0,
		zM20,
		1);

    zgenerics_draw(glands);

    zglands_delete(Z_GLANDS(glands));
    zdevice_delete(&dev);

    return 0;
}

int create_note(int argc, char** argv)
{
    zdevice dev;
    zgeneric* note;

    zdevice_new2(zSheetA3_Landscape,
		 0,
		 &dev);

    note = znote_new(NULL);
    zgeneric_set_device(note, &dev);
    zgeneric_set_default_dev_context(note);
    zbase_set_base_coords(Z_BASE(note), 100.0, 200.0);

    znote_set_content(Z_NOTE(note), "TESTING", 0);
    znote_draw(Z_NOTE(note));

    zgeneric_delete(note);
    zdevice_delete(&dev);

    return 0;
}

int create_jb(zfile* obj)
{
    zdevice dev;
    zgeneric* _jb = NULL;
    const char* _buff = NULL;
    
    zdevice_new2(zSheetA3_Landscape,
		 0,
		 &dev);

    _jb = zjb_new(NULL, &dev, 100.0, 150.0, 80.0, 80.0, 20.0, 0.0);
    zjb_set_fillet_radius(Z_JB(_jb), 5.0);
    
    zjb_draw(Z_JB(_jb));

    /* get buffer */
    _buff = zdevice_get_temp_buff(&dev);

    /* add to the file object */
    zfile_parse_and_insert_elements(obj, _buff);    

    zjb_delete(Z_JB(_jb));
    zdevice_delete(&dev);

    return 0;
}
