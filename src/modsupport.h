#ifndef Py_MODSUPPORT_H
#define Py_MODSUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "methodobject.h"

/* Module support interface */

extern object *initmodule(char *, struct methodlist *);
extern int getintarg(object *v, int *a);
extern int getnoarg(object *v);
extern int getstrarg(object *v, object **a);
extern int getstrstrarg(object *v, object **a, object **b);
extern int getstrintarg(object *v, object **a, int *b);
extern int getlongarg(object *v, long *a);
extern int getlonglongargs(object *v, long *a, long *b);

#ifdef __cplusplus
}
#endif

#endif /* !Py_MODSUPPORT_H */
