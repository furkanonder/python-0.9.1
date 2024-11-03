/* Function object interface */

extern typeobject Functype;

#define is_funcobject(op) ((op)->ob_type == &Functype)

extern object *newfuncobject(object *, object *);
extern object *getfunccode(object *);
extern object *getfuncglobals(object *);
