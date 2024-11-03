#ifndef Py_TUPLEOBJECT_H
#define Py_TUPLEOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Tuple object interface */

/* 123456789-123456789-123456789-123456789-123456789-123456789-123456789-12

Another generally useful object type is an tuple of object pointers.
This is a mutable type: the tuple items can be changed (but not their
number).  Out-of-range indices or non-tuple objects are ignored.

*** WARNING *** settupleitem does not increment the new item's reference
count, but does decrement the reference count of the item it replaces,
if not nil.  It does *decrement* the reference count if it is *not*
inserted in the tuple.  Similarly, gettupleitem does not increment the
returned item's reference count. */

typedef struct {
	OB_VARHEAD
	object *ob_item[1];
} tupleobject;

extern typeobject Tupletype;

#define is_tupleobject(op) ((op)->ob_type == &Tupletype)

extern object *newtupleobject(int size);
extern int gettuplesize(object *);
extern object *gettupleitem(object *, int);
extern int settupleitem(object *, int, object *);

/* Macro, trading safety for speed */
#define GETTUPLEITEM(op, i) ((op)->ob_item[i])

#ifdef __cplusplus
}
#endif

#endif /* !Py_TUPLEOBJECT_H */
