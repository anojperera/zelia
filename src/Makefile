##  make file
## Sun Oct 17 13:02:32 BST 2010

CFLAGS1 = -g
CFLAGS2 = -O0
CFLAGS3 = -Wall -pedantic
PROG = tmp
CCC = gcc
CCP = g++
OBJS = main.o zDevice.o zGeneric.o zSheet.o zBase.o zTerminal.o zGenerics.o \
zTerminals.o zJB.o zGland.o zGlands.o zArrow.o zLeader.o zTCell.o zTCells.o \
zTRow.o zTRows.o zTable.o zNote.o zNotes.o
LIBSP = ../../g_list/bin/alist.a
GTKOBJS = `pkg-config --libs gtk+-2.0`
GTKCFLGS = `pkg-config --cflags gtk+-2.0`
$(PROG) : $(OBJS)
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -o ../bin/$(PROG) \
$(OBJS) $(GTKOBJS) $(LIBSP)

main.o : main.c 
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c main.c $(GTKCFLGS)

zDevice.o : zDevice.c zDevice.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zDevice.c $(GTKCFLGS)

zGeneric.o : zGeneric.c zGeneric.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zGeneric.c $(GTKCFLGS)

zSheet.o : zSheet.c zSheet.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zSheet.c $(GTKCFLGS)

zBase.o : zBase.c zBase.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zBase.c $(GTKCFLGS)

zTerminal.o : zTerminal.c zTerminal.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zTerminal.c $(GTKCFLGS)

zGenerics.o : zGenerics.c zGenerics.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zGenerics.c $(GTKCFLGS)

zTerminals.o : zTerminals.c zTerminals.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zTerminals.c $(GTKCFLGS)

zJB.o : zJB.c zJB.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zJB.c $(GTKCFLGS)

zGland.o : zGland.c zGland.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zGland.c $(GTKCFLGS)

zGlands.o : zGlands.c zGlands.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zGlands.c $(GTKCFLGS)

zArrow.o : zArrow.c zArrow.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zArrow.c $(GTKCFLGS)

zLeader.o : zLeader.c zLeader.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zLeader.c $(GTKCFLGS)

zTCell.o : zTCell.c zTCell.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zTCell.c $(GTKCFLGS)

zTCells.o : zTCells.c zTCells.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zTCells.c $(GTKCFLGS)

zTRow.o : zTRow.c zTRow.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zTRow.c $(GTKCFLGS)

zTRows.o : zTRows.c zTRows.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zTRows.c $(GTKCFLGS)

zTable.o : zTable.c zTable.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zTable.c $(GTKCFLGS)

zNote.o : zNote.c zNote.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zNote.c $(GTKCFLGS)

zNotes.o : zNotes.c zNotes.h
		$(CCC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c zNotes.c $(GTKCFLGS)

clean :
	rm -fv *.o
