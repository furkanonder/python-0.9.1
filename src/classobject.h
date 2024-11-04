#ifndef Py_CLASSOBJECT_H
#define Py_CLASSOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Class object interface */

/* Classes are really hacked in at the last moment. It should be possible to
use other object types as base classes, but currently it isn't.  We'll see
if we can fix that later, sigh... */

extern typeobject Classtype, Classmembertype, Classmethodtype;

#define is_classobject(op)           ((op)->ob_type == &Classtype)
#define is_classmemberobject(op)     ((op)->ob_type == &Classmembertype)
#define is_classmethodobject(op)     ((op)->ob_type == &Classmethodtype)

extern object *newclassobject(object *, object *);
extern object *newclassmemberobject(object *);
extern object *newclassmethodobject(object *, object *);
extern object *classmethodgetfunc(object *);
extern object *classmethodgetself(object *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_CLASSOBJECT_H */
