/*
The macro PROTO(x) is used to put function prototypes in the source.
This is defined differently for compilers that support prototypes than
for compilers that don't.  It should be used as follows:
	int some_function PROTO((int arg1, char *arg2));
A variant FPROTO(x) is used for cases where Standard C allows prototypes
but Think C doesn't (mostly function pointers).

This file also defines the macro HAVE_PROTOTYPES if and only if
the PROTO() macro expands the prototype.  It is also allowed to predefine
HAVE_PROTOTYPES to force prototypes on.
*/

#ifndef PROTO

#ifdef sgi
#ifdef mips
#define HAVE_PROTOTYPES
#endif
#endif

#ifdef HAVE_PROTOTYPES
#define PROTO(x) x
#else
#define PROTO(x) ()
#endif

#endif /* PROTO */

#ifndef FPROTO
#define FPROTO(x) PROTO(x)
#endif