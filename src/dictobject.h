#ifndef Py_DICTOBJECT_H
#define Py_DICTOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Dictionary object type -- mapping from char * to object.  NB: the key is
given as a char *, not as a stringobject.  These functions set errno for
errors.  Functions dictremove() and dictinsert() return nonzero for errors,
getdictsize() returns -1, the others NULL.  A successful call to dictinsert()
calls INCREF() for the inserted item. */

extern typeobject Dicttype;
#define is_dictobject(op) ((op)->ob_type == &Dicttype)

extern object    *newdictobject(void);
extern object    *dictlookup(object *dp, char *key);
extern int       dictinsert(object *dp, char *key, object *item);
extern int       dictremove(object *dp, char *key);
extern int       getdictsize(object *dp);
extern char      *getdictkey(object *dp, int i);
extern object    *getdictkeys(object *dp);

#ifdef __cplusplus
}
#endif

#endif /* !Py_DICTOBJECT_H */
