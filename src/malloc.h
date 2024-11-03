/* Lowest-level memory allocation interface */

#ifdef __STD_C__
#define ANY void
#define HAVE_STDLIB
#endif

#ifdef __STDC__
#define ANY void
#define HAVE_STDLIB
#endif

#ifndef ANY
#define ANY char
#endif

#ifndef NULL
#define NULL 0
#endif

#define NEW(type, n) ( (type *) malloc((n) * sizeof(type)) )
#define RESIZE(p, type, n) \
	if ((p) == NULL) \
		(p) =  (type *) malloc((n) * sizeof(type)); \
	else \
		(p) = (type *) realloc((char *)(p), (n) * sizeof(type))
#define DEL(p) free((char *)p)
#define XDEL(p) if ((p) == NULL) ; else DEL(p)

#ifdef HAVE_STDLIB
#include <stdlib.h>
#else
extern ANY *malloc PROTO((unsigned int));
extern ANY *calloc PROTO((unsigned int, unsigned int));
extern ANY *realloc PROTO((ANY *, unsigned int));
extern void free PROTO((ANY *)); /* XXX sometimes int on Unix old systems */
#endif
