# Makefile for Python
# ===================
#
# If you are in a hurry, you can just edit this Makefile to choose the
# correct settings for SYSV and RANLIB below, and type "make" in this
# directory.  If you are using a recent version of SunOS (or Ultrix?)
# you don't even have to edit: the Makefile comes pre-configured for
# such systems with all configurable options turned off, building the
# minimal portable version of the Python interpreter.
#
# If have more time, read the section on configurable options below.
# It may still be wise to begin building the minimal portable Python,
# to see if it works at all, and select options later.  You don't have
# to rebuild all objects when you turn on options; all dependencies
# are concentrated in the file "config.c" which is rebuilt whenever
# the Makefile changes.  (Except if you turn on the GNU Readline option
# you may have to toss out the tokenizer.o object.)

# Operating System Defines (ALWAYS READ THIS)
# ===========================================

# Uncomment the following line if you are using a System V derivative.
# This must be used, for instance, on an SGI IRIS.  Don't use it for
# SunOS.  (This is only needed by posixmodule.c...)

#SYSVDEF=	-DSYSV

# Choose one of the following two lines depending on whether your system
# requires the use of 'ranlib' after creating a library, or not.

#RANLIB =	true	# For System V
RANLIB =	ranlib	# For BSD

# If your system doesn't have symbolic links, uncomment the following
# line.

#NOSYMLINKDEF=	-DNO_LSTAT

# Installation Options
# ====================

# You may want to change DEFPYTHONPATH to reflect where you install the
# Python module library. The default contains "../lib" so running
# the interpreter from the source/build directory as distributed will
# find the library (admittedly a hack).

DEFPYTHONPATH= .:/usr/local/lib/python:/ufs/guido/lib/python:../lib

# For "Pure" BSD Systems
# ======================
#
# "Pure" BSD systems (as opposed to enhanced BSD derivatives like SunOS)
# often miss certain standard library functions.  Source for
# these is provided, you just have to turn it on.  This may work for
# other systems as well, where these things are needed.

# If your system does not have a strerror() function in the library,
# uncomment the following two lines to use one I wrote.  (Actually, this
# is missing in most systems I have encountered, so it is turned on
# in the Makefile.  Turn it off if your system doesn't have sys_errlist.)

# STRERROR_SRC=  strerror.c
# STRERROR_OBJ=  strerror.o

# If your BSD system does not have a fmod() function in the library,
# uncomment the following two lines to use one I wrote.

#FMOD_SRC=  fmod.c
#FMOD_OBJ=  fmod.o

# If your BSD system does not have a strtol() function in the library,
# uncomment the following two lines to use one I wrote.

#STRTOL_SRC=  strtol.c
#STRTOL_OBJ=  strtol.o

# If your BSD system does not have a getcwd() function in the library,
# but it does have a getwd() function, uncomment the following two lines
# to use one I wrote.  (If you don't have getwd() either, turn on the
# NO_GETWD #define in getcwd.c.)

#GETCWD_SRC=  getcwd.c
#GETCWD_OBJ=  getcwd.o

# If your signal() function believes signal handlers return int,
# uncomment the following line.

#SIGTYPEDEF=	-DSIGTYPE=int

# Further porting hints
# =====================
#
# If you don't have the header file <string.h>, but you do have
# <strings.h>, create a file "string.h" in this directory which contains
# the single line "#include <strings.h>", and add "-I." to CFLAGS.
# If you don't have the functions strchr and strrchr, add definitions
# "-Dstrchr=index -Dstrrchr=rindex" to CFLAGS.  (NB: CFLAGS is not
# defined in this Makefile.)

# Configurable Options
# ====================
#
# Python can be configured to interface to various system libraries that
# are not available on all systems.  It is also possible to configure
# the input module to use the GNU Readline library for interactive
# input.  For each configuration choice you must uncomment the relevant
# section of the Makefile below.  Note: you may also have to change a
# pathname and/or an architecture identifier that is hardcoded in the
# Makefile.
#
# Read the comments to determine if you can use the option.  (You can
# always leave all options off and build a minimal portable version of
# Python.)

# GNU Readline Option
# ===================
#
# If you have the sources of the GNU Readline library you can have
# full interactive command line editing and history in Python.
# The GNU Readline library is distributed with the BASH shell
# (I only know of version 1.05).  You must build the GNU Readline
# library and the alloca routine it needs in their own source
# directories (which are subdirectories of the bash source directory),
# and plant a pointer to the BASH source directory in this Makefile.
#
# Uncomment and edit the following block to use the GNU Readline option.
LIBREADLINE = -lreadline
RL_USE 		= -DUSE_READLINE
RL_LIBS		= $(LIBREADLINE)
RL_LIBDEPS	= $(LIBREADLINE)

# Major Definitions
# =================

STANDARD_OBJ = acceler.o bltinmodule.o ceval.o classobject.o \
		compile.o dictobject.o errors.o fgetsintr.o \
		fileobject.o floatobject.o $(FMOD_OBJ) frameobject.o \
		funcobject.o $(GETCWD_OBJ) \
		graminit.o grammar1.o import.o \
		intobject.o intrcheck.o listnode.o listobject.o \
		mathmodule.o methodobject.o modsupport.o \
		moduleobject.o node.o object.o parser.o \
		parsetok.o posixmodule.o regexp.o regexpmodule.o \
		$(STRERROR_OBJ) \
		stringobject.o $(STRTOL_OBJ) structmember.o \
		sysmodule.o timemodule.o tokenizer.o traceback.o \
		tupleobject.o typeobject.o

STANDARD_SRC = acceler.c bltinmodule.c ceval.c classobject.c \
		compile.c dictobject.c errors.c fgetsintr.c \
		fileobject.c floatobject.c $(FMOD_SRC) frameobject.c \
		funcobject.c $(GETCWD_SRC) \
		graminit.c grammar1.c import.c \
		intobject.c intrcheck.c listnode.c listobject.c \
		mathmodule.c methodobject.c modsupport.c \
		moduleobject.c node.c object.c parser.c \
		parsetok.c posixmodule.c regexp.c regexpmodule.c \
		$(STRERROR_SRC) \
		stringobject.c $(STRTOL_SRC) structmember.c \
		sysmodule.c timemodule.c tokenizer.c traceback.c \
		tupleobject.c typeobject.c

CONFIGDEFS=	$(STDW_USE) $(AM_USE) $(AUDIO_USE) $(GL_USE) $(PANEL_USE) \
		'-DPYTHONPATH="$(DEFPYTHONPATH)"'

CONFIGINCLS=	$(STDW_INCL)

LIBDEPS=	libpython.a $(STDW_LIBDEPS) $(AM_LIBDEPS) \
		$(GL_LIBDEPS) $(PANEL_LIBSDEP) $(RL_LIBDEPS)

# NB: the ordering of items in LIBS is significant!
LIBS=		libpython.a $(STDW_LIBS) $(AM_LIBS) \
		$(PANEL_LIBS) $(GL_LIBS) $(RL_LIBS) -lm

LIBOBJECTS=	$(STANDARD_OBJ) $(STDW_OBJ) $(AM_OBJ) $(AUDIO_OBJ) \
		$(GL_OBJ) $(PANEL_OBJ)

LIBSOURCES=	$(STANDARD_SRC) $(STDW_SRC) $(AM_SRC) $(AUDIO_SRC) \
		$(GL_SRC) $(PANEL_SRC)

OBJECTS=	pythonmain.o config.o

SOURCES=	$(LIBSOURCES) pythonmain.c config.c

GENOBJECTS=	acceler.o fgetsintr.o grammar1.o \
		intrcheck.o listnode.o node.o parser.o \
		parsetok.o tokenizer.o bitset.o \
		firstsets.o grammar.o metagrammar.o pgen.o \
		pgenmain.o printgrammar.o

GENSOURCES=	acceler.c fgetsintr.c grammar1.c \
		intrcheck.c listnode.c node.c parser.c \
		parsetok.c tokenizer.c bitset.c \
		firstsets.c grammar.c metagrammar.c pgen.c \
		pgenmain.c printgrammar.c


# Main Targets
# ============

python:		libpython.a $(OBJECTS) $(LIBDEPS) Makefile
		$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o python

libpython.a:	$(LIBOBJECTS)
		-rm -f @lib
		ar cr @lib $(LIBOBJECTS)
		$(RANLIB) @lib
		mv @lib libpython.a

python_gen:	$(GENOBJECTS) $(RL_LIBDEPS)
		$(CC) $(CFLAGS) $(GENOBJECTS) $(RL_LIBS) -o python_gen


# Utility Targets
# ===============

# Don't take the output from lint too seriously.  I have not attempted
# to make Python lint-free.  But I use function prototypes.

LINTFLAGS=	-h

LINTCPPFLAGS=	$(CONFIGDEFS) $(CONFIGINCLS) $(SYSVDEF) \
		$(AM_INCL) $(PANEL_INCL)

LINT=		lint

lint::		$(SOURCES)
		$(LINT) $(LINTFLAGS) $(LINTCPPFLAGS) $(SOURCES)

lint::		$(GENSOURCES)
		$(LINT) $(LINTFLAGS) $(GENSOURCES)

# Generating dependencies is only necessary if you intend to hack Python.
# You may change $(MKDEP) to your favorite dependency generator (it should
# edit the Makefile in place).

MKDEP=		mkdep

depend::
		$(MKDEP) $(LINTCPPFLAGS) $(SOURCES) $(GENSOURCES)

# You may change $(CTAGS) to suit your taste...

CTAGS=		ctags -t -w

HEADERS=	*.h

tags:		$(SOURCES) $(GENSOURCES) $(HEADERS)
		$(CTAGS) $(SOURCES) $(GENSOURCES) $(HEADERS)

clean::
		-rm -f *.o core [,#@]*

clobber::	clean
		-rm -f python python_gen libpython.a tags


# Build Special Objects
# =====================

# You may change $(COMPILE) to reflect the default .c.o rule...

COMPILE=	$(CC) -c $(CFLAGS)

config.o:	config.c Makefile
		$(COMPILE) $(CONFIGDEFS) $(CONFIGINCLS) $*.c

fgetsintr.o:	fgetsintr.c
		$(COMPILE) $(SIGTYPEDEF) $*.c

intrcheck.o:	intrcheck.c
		$(COMPILE) $(SIGTYPEDEF) $*.c

panelmodule.o:	panelmodule.c
		$(COMPILE) $(PANEL_INCL) $*.c

posixmodule.o:	posixmodule.c
		$(COMPILE) $(SYSVDEF) $(NOSYMLINKDEF) $*.c

sc_interpr.o:	sc_interpr.c
		$(COMPILE) $(AM_INCL) $*.c

sc_error.o:	sc_error.c
		$(COMPILE) $(AM_INCL) $*.c

timemodule.o:	timemodule.c
		$(COMPILE) $(SIGTYPEDEF) $(BSDTIMEDEF) $*.c

tokenizer.o:	tokenizer.c
		$(COMPILE) $(RL_USE) $*.c

.PRECIOUS:	python libpython.a glmodule.c graminit.c graminit.h

# Generated Sources
# =================
#
# Some source files are (or may be) generated.
# The rules for doing so are given here.

# The dependencies for graminit.[ch] are not turned on in the
# distributed Makefile because the files themselves are distributed.
# Turn them on if you want to hack the grammar.

#graminit.c graminit.h:	Grammar python_gen
#		python_gen Grammar
