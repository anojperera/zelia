##  make file
## Sun Oct 17 13:02:32 BST 2010

CFLAGS1 = -g
CFLAGS2 = -O0
CFLAGS3 = -Wall
PROG = tmp
CCC = gcc
CCP = g++
OBJS = main.o zDevice.o zGeneric.o zSheet.o
## LIBSP = ../../g_list/bin/alist.a
GTKOBJS = `pkg-config --libs gtk+-2.0`
GTKCFLGS = `pkg-config --cflags gtk+-2.0`
$(PROG) : $(OBJS)
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -o $(PROG) $(OBJS) $(GTKOBJS)

main.o : main.c 
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c main.c $(GTKCFLGS)

zDevice.o : zDevice.c zDevice.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zDevice.c $(GTKCFLGS)

zGeneric.o : zGeneric.c zGeneric.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zGeneric.c $(GTKCFLGS)

zSheet.o : zSheet.c zSheet.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zSheet.c $(GTKCFLGS)

clean :
	rm -fv *.o
