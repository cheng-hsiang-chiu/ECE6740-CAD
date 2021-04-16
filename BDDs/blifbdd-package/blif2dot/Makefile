#
# BDDlopt: BDD Decomposition-Based Logic Optimization Program
#---------------------------------------------------------------------------
.SUFFIXES: .o .c .u

CC	= gcc
#CC	= cc
RANLIB	= ranlib
#RANLIB	= :

PURE   = 
#PURE	= /usr/local/bin/purify
PUREDOT =
#PUREDOT = .pure
QUAN	=
#QUAN	= /net/iss/usr/local/bin/quantify-3.1.1-solaris2/quantify
QUANDOT =
#QUANDOT = .quan
#LINK   = 
LINK	= $(PURE)$(QUAN)

TARGET	= blif2dot$(PUREDOT)$(QUANDOT)

file    = ../cudd_location
WHERE	= $(shell cat ${file})

INCLUDE = $(WHERE)/include
LIBS	= $(WHERE)/dddmp/libdddmp.a $(WHERE)/cudd/libcudd.a \
	$(WHERE)/mtr/libmtr.a $(WHERE)/st/libst.a $(WHERE)/util/libutil.a \
	$(WHERE)/epd/libepd.a

#LIBS	= $(WHERE)/st/libst.a $(WHERE)/util/libutil.a \
#		  $(WHERE)/cudd/libcudd.a $(WHERE)/epd/libepd.a $(WHERE)/mtr/libmtr.a $(WHERE)/dddmp/libdddmp.a

BLIBS	= -kL$(WHERE)/st -klst -kL$(WHERE)/util -klutil

LINTLIBS = $(WHERE)/st/llib-lst.ln $(WHERE)/util/llib-lutil.ln

SRC	= main.c bnet.c 
HDR	= bnet.h 
OBJ	= $(SRC:.c=.o)
UBJ	= $(SRC:.c=.u)

#ICFLAGS = -g -O6 -Wall
#ICFLAGS = -g -Wall
#ICFLAGS = -g
ICFLAGS = -O3

XCFLAGS = -DHAVE_IEEE_754 -DBSD

CFLAGS	= $(ICFLAGS) $(MFLAG) $(XCFLAGS)

LINTFLAGS = -u -n -DDEBUG 

#------------------------------------------------------

$(TARGET): $(SRC) $(OBJ) $(HDR) $(LIBS) $(MNEMLIB)
	$(LINK) $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LIBS) $(MNEMLIB) -lm

.c.o: $(HDR)
	$(CC) -c $< -I$(INCLUDE) $(CFLAGS) $(DDDEBUG)

# if the header files change, recompile
$(OBJ): $(HDR)
$(UBJ): $(HDR)

.c.u: $(SRC) $(HDR)
	cc -j -I$(INCLUDE) $(XCFLAGS) $<

lint: $(SRC) $(HDR) $(LINTLIBS)
	lint $(LINTFLAGS) -I$(INCLUDE) $(SRC) $(LINTLIBS)

tags: $(SRC) $(HDR)
	ctags $(SRC) $(HDR)

all: $(TARGET) lint tags

clean:
	rm -f *.o 

distclean: clean
	rm -f $(TARGET) *~ *.qv *.qx


