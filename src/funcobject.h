#ifndef Py_FUNCOBJECT_H
#define Py_FUNCOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Function object interface */

extern typeobject Functype;
#define is_funcobject(op) ((op)->ob_type == &Functype)

extern object *newfuncobject(object *, object *);
extern object *getfunccode(object *);
extern object *getfuncglobals(object *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_FUNCOBJECT_H */
