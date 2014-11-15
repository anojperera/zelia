/* Temporary function to opern and draw a box and save */

#include <stdlib.h>
#include <stdio.h>
#include "zVar.h"
#include "zfile.h"
#include "zfile_attrib.h"

#define Z_FILE_PATH "/home/pyrus/Prog/C++/zelia/resources/wozair_border_a4.svg"
#define Z_FILE_SAVE_PATH "/home/pyrus/temp.svg"
#define Z_FILE_XML_ATTRIB_PATH "/home/pyrus/Prog/C++/zelia/src/attrib.xml"

#define Z_FILE_ATTRIB_ARRAY			\
    {						\
	{ "drawing_no", "C123456"},		\
	{ "sheet_no", "2"},			\
	{NULL, NULL}				\
    }

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


    /* clean up */
    zfile_attrib_delete(&_attrib);
    zfile_delete(&_file);
    return 0;
}


