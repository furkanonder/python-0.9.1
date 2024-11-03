/* Module support interface */

extern object *initmodule PROTO((char *, struct methodlist *));
extern int getintarg(object *v, int *a);
extern int getnoarg(object *v);
extern int getstrarg(object *v, object **a);
extern int getstrstrarg(object *v, object **a, object **b);
extern int getstrintarg(object *v, object **a, int *b);
extern int getlongarg(object *v, long *a);
extern int getlonglongargs(object *v, long *a, long *b);
extern void fatal(char *msg);
