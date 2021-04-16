# filename: Makefile
# zzeng: 11/16/98 : copied from the Nanotrav with Cudd-2.3.0 
# zzeng: modified

.SUFFIXES: .o .c .u

CC      = gcc
#CC     = cc
RANLIB  = ranlib
#RANLIB = :
PURE    =

EXE     =
#EXE    = .exe
TARGET  = blif2bdd$(EXE)

file = ../cudd_location
WHERE   = $(shell cat ${file})

INCLUDE = $(WHERE)/include

LIBS    = $(WHERE)/dddmp/libdddmp.a \
		$(WHERE)/cudd/libcudd.a \
		$(WHERE)/epd/libepd.a \
	    $(WHERE)/mtr/libmtr.a $(WHERE)/st/libst.a $(WHERE)/util/libutil.a

SRC 	= main.c bnet.c ntr.c  ntrHeap.c ntrMflow.c
HDR     = bnet.h ntr.h $(WHERE)/include/dddmp.h $(WHERE)/include/cudd.h \
        $(WHERE)/include/cuddInt.h

OBJ     = $(SRC:.c=.o)
UBJ     = $(SRC:.c=.u)

DDDEBUG =  
#XFLAGS 	= -mcpu=pentiumpro -malign-double -DHAVE_IEEE_754
XFLAGS 	=  -malign-double -DHAVE_IEEE_754

ICFLAGS = -g $(XFLAGS)
ICFLAGS = -O6 $(XFLAGS)

$(TARGET): $(SRC) $(OBJ)
	$(CC) $(ICFLAGS) -I$(INCLUDE) -o $@ $(OBJ) $(LIBS) -lm

.c.o: $(HDR)
	$(CC) -c $< -I$(INCLUDE) $(ICFLAGS) $(DDDEBUG)

clean:
	rm -f *.o *.u mon.out gmon.out *.pixie *.Addrs *.Counts mnem.* \
	.pure core *.warnings tags

distclean: clean
	rm -f $(TARGET) *.bak *~ .gdb_history *.qv *.qx
