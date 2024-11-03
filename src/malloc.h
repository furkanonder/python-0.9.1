/* Lowest-level memory allocation interface */
#include <stdlib.h>

#ifndef ANY
#define ANY char
#endif

#ifndef NULL
#define NULL 0
#endif

#define NEW(type, n) ((type *)malloc((n) * sizeof(type)))
#define RESIZE(p, type, n) \
	if ((p) == NULL) \
		(p) =  (type *)malloc((n) * sizeof(type)); \
	else \
		(p) = (type *) realloc((char *)(p), (n) * sizeof(type))
#define DEL(p) free((char *)p)
#define XDEL(p) if ((p) == NULL) ; else DEL(p)
