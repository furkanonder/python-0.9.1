#ifndef Py_MODULEOBJECT_H
#define Py_MODULEOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Module object interface */

extern typeobject Moduletype;

#define is_moduleobject(op) ((op)->ob_type == &Moduletype)

extern object *newmoduleobject(char *);
extern object *getmoduledict(object *);
extern char *getmodulename(object *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_MODULEOBJECT_H */
