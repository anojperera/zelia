#!/bin/bash
# make shared object
# creating shared object
gcc -g -Wall -O0 -fPIC -pedantic -c *.c -D_GNU_SOURCE -I/usr/include/libxml2/ `pkg-config --cflags gtk+-2.0`
rm main.o
mv *.o ../bin/
gcc -shared -Wl,-soname,libzelia.so -o ../bin/libzelia.so.1.0.1 ../bin/*.o
rm ../bin/*.o

gcc -g -Wall -pg -O0 -pedantic -o ../bin/zelia main.c -L/home/pyrus/Prog/C++/zelia/bin/ -lzelia \
    -I/usr/include/libxml2/ `pkg-config --cflags --libs gtk+-2.0` \
    -lxml2 -lalist
echo "complete"

exit 0
