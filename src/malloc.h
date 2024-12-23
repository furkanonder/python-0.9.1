#ifndef Py_MYMALLOC_H
#define Py_MYMALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Lowest-level memory allocation interface */
#include <stdlib.h>

#ifndef ANY
#define ANY char
#endif

#define NEW(type, n) ((type *)malloc((n) * sizeof(type)))
#define RESIZE(p, type, n) \
	if ((p) == NULL) \
		(p) =  (type *)malloc((n) * sizeof(type)); \
	else \
		(p) = (type *)realloc((char *)(p), (n) * sizeof(type))
#define DEL(p) free((char *)p)
#define XDEL(p) if ((p) == NULL) ; else DEL(p)

#ifdef __cplusplus
}
#endif

#endif /* !Py_MYMALLOC_H */
