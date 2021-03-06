/*
 * labels defined for helping the parser
 */

#ifndef _ZELIA_PARSER_H_
#define _ZELIA_PARSER_H_

#include <blist.h>
#include "zfile.h"
#include "zfile_attrib.h"
#include "zgeneric.h"
#include "zgenerics.h"

#define ZPARSER_FILE "file"
#define ZPARSER_TEMPLATE_PATH "template_path"
#define ZPARSER_SAVE_PATH "save_path"
#define ZPARSER_SHEET_SIZE "sheet_size"
#define ZPARSER_SHEET_ATTRIB "attrib"
#define ZPARSER_TABLE "table"
#define ZPARSER_COORD_X "x"
#define ZPARSER_COORD_Y "y"
#define ZPARSER_TABLE_ROWS "rows"
#define ZPARSER_TABLE_ROW "row"
#define ZPARSER_TABLE_COLUMNS "columns"
#define ZPARSER_TABLE_COLUMN "column"
#define ZPARSER_COLUMN_WIDTH_ATTRIB "width"
#define ZPARSER_ROW_HEIGHT_ATTRIB "height"
#define ZPARSER_DEPTH "depth"
#define ZPARSER_RADIUS "radius"
#define ZPARSER_ANGLE "angle"
#define ZPARSER_ROW_IX_ATTRIB "row_ix"
#define ZPARSER_COL_IX_ATTRIB "col_ix"
#define ZPARSER_CONTENT "content"
#define ZPARSER_NOTES "notes"
#define ZPARSER_NOTE "note"
#define ZPARSER_TITLE "title"
#define ZPARSER_JB "jb"
#define ZPARSER_BASE "base"
#define ZPARSER_GLAND "gland"
#define ZPARSER_GLANDS "glands"
#define ZPARSER_TERMINAL "terminal"
#define ZPARSER_TERMINALS "terminals"
#define ZPARSER_TERMINALS_NUM "num_terminals"
#define ZPARSER_ETERMINALS_NUM "num_eterminals"
#define ZPARSER_TERMINAL_ANNOT "terminal_annotation"
#define ZPARSER_TERMINALS_LINKS "links"
#define ZPARSER_TABLE_TCELL "cell"
#define ZPARSER_TABLE_CELLS "cells"
#define ZPARSER_GENERIC "generic"
#define ZPARSER_GENERICS "generics"
#define ZPARSER_SIZE "size"
#define ZPARSER_HEX_FLG "hex"
#define ZPARSER_LEADER "leader"
#define ZPARSER_ARROW "arrow"
#define ZPARSER_SEGMENT_1 "seg1"
#define ZPARSER_SEGMENT_2 "seg2"
#define ZPARSER_DESCRIPTION "desc"
#define ZPARSER_LABEL "label"

#define ZPARSER_SHEET_A4_PORT "A4_P"
#define ZPARSER_SHEET_A4_LAND "A4_L"
#define ZPARSER_SHEET_A3_PORT "A3_P"
#define ZPARSER_SHEET_A3_LAND "A3_L"

#define ZPARSER_GLAND_SZ_M16 "M16"
#define ZPARSER_GLAND_SZ_M20 "M20"
#define ZPARSER_GLAND_SZ_M25 "M25"

struct _zobject
{
    enum _zobject_primary_type
	{
	    zobject_item,
	    zobject_cols
	} type;

    union _zobject_data
    {
	zgeneric* _i;
	zgenerics* _c;
    } data;
};

struct _zparser
{
    unsigned int _init_flg;
    zfile file;
    zfile_attrib attrib;
    zdevice device;
    blist object_array;
};

#endif /* _ZELIA_PARSER_H_ */
