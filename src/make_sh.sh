#!/bin/bash
gcc -g -Wall -pg -O0 -pedantic -o zelia main.c zelia_xml.c zfile.c zfile_attrib.c \
    zbase.c zdevice.c zgeneric.c zgenerics.c zgland.c zglands.c \
    zjb.c znote.c znotes.c ztable.c ztcell.c ztcells.c zterminal.c \
    zterminals.c ztrow.c ztrows.c zarrow.c zleader.c -D_GNU_SOURCE \
    -I/usr/include/libxml2/ `pkg-config --cflags --libs gtk+-2.0` \
    -lxml2 -lalist
exit 0
