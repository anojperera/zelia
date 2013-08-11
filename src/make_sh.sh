#!/bin/bash
gcc -g -Wall -O0 -pedantic -o zelia *.c `pkg-config --cflags --libs librsvg-2.0`
exit 0