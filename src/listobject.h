#ifndef Py_LISTOBJECT_H
#define Py_LISTOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* List object interface */

/* 123456789-123456789-123456789-123456789-123456789-123456789-123456789-12

Another generally useful object type is an list of object pointers.
This is a mutable type: the list items can be changed, and items can be
added or removed.  Out-of-range indices or non-list objects are ignored.

*** WARNING *** setlistitem does not increment the new item's reference
count, but does decrement the reference count of the item it replaces,
if not nil.  It does *decrement* the reference count if it is *not*
inserted in the list.  Similarly, getlistitem does not increment the
returned item's reference count.
*/

typedef struct {
	OB_VARHEAD
	object **ob_item;
} listobject;

extern typeobject Listtype;

#define is_listobject(op) ((op)->ob_type == &Listtype)

extern object *newlistobject(int size);
extern int getlistsize(object *);
extern object *getlistitem(object *, int);
extern int setlistitem(object *, int, object *);
extern int inslistitem(object *, int, object *);
extern int addlistitem(object *, object *);
extern int sortlist(object *);

/* Macro, trading safety for speed */
#define GETLISTITEM(op, i) ((op)->ob_item[i])

#ifdef __cplusplus
}
#endif

#endif /* !Py_LISTOBJECT_H */
