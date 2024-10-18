/* Module support interface */

extern object *initmodule PROTO((char *, struct methodlist *));
extern int getintarg(object *v, int *a);
extern void fatal(char *msg);
