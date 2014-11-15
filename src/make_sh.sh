#!/bin/bash
gcc -g -Wall -O0 -pedantic -o zelia main.c zfile.c zfile_attrib.c -I/usr/include/libxml2/ -lxml2
exit 0
