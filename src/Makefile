CC = cc
CFLAGS = -c
AR = ar
RANLIB = ranlib
LIBS = -lm
DEFPYTHONPATH = '-DPYTHONPATH=".:../lib"'

STANDARD_OBJ = acceler.o bltinmodule.o ceval.o classobject.o compile.o \
	dictobject.o errors.o fgetsintr.o fileobject.o floatobject.o \
	frameobject.o funcobject.o graminit.o grammar1.o import.o intobject.o \
	intrcheck.o listnode.o listobject.o mathmodule.o methodobject.o \
	modsupport.o moduleobject.o node.o object.o parser.o parsetok.o \
	posixmodule.o regexp.o regexpmodule.o stringobject.o structmember.o \
    sysmodule.o timemodule.o tokenizer.o traceback.o tupleobject.o \
    typeobject.o

GENOBJECTS = acceler.o fgetsintr.o grammar1.o intrcheck.o listnode.o node.o \
		parser.o parsetok.o tokenizer.o bitset.o firstsets.o grammar.o \
		metagrammar.o pgen.o pgenmain.o printgrammar.o

GENSOURCES = acceler.c fgetsintr.c grammar1.c intrcheck.c listnode.c node.c \
		parser.c parsetok.c tokenizer.c bitset.c firstsets.c grammar.c \
		metagrammar.c pgen.c pgenmain.c printgrammar.c

all: python

python_gen: $(GENOBJECTS)
	$(CC) $(GENSOURCES) -o python_gen
	./python_gen Grammar

python: python_gen libpython.a pythonmain.o config.o
	$(CC) pythonmain.o config.o libpython.a $(LIBS) -o python

libpython.a: $(STANDARD_OBJ)
	-rm -f @lib
	$(AR) cr @lib $(STANDARD_OBJ)
	$(RANLIB) @lib
	mv @lib libpython.a

pythonmain.o: pythonmain.c
	$(CC) $(CFLAGS) -o pythonmain.o pythonmain.c

config.o: config.c
	$(CC) $(CFLAGS) $(DEFPYTHONPATH) -o config.o config.c

clean:
	-rm -f *.o python libpython.a python_gen graminit.c graminit.h

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

test: python
	./python ../lib/testall.py

.PHONY: all clean
