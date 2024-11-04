/* Module support implementation */

#include <string.h>

#include "object.h"
#include "intobject.h"
#include "stringobject.h"
#include "tupleobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "methodobject.h"
#include "moduleobject.h"
#include "errors.h"
#include "modsupport.h"
#include "import.h"

object *
initmodule(char *name, struct methodlist *methods)
{
	object *m, *d, *v;
	struct methodlist *ml;
	char namebuf[256];

	if ((m = add_module(name)) == NULL) {
		fprintf(stderr, "initializing module: %s\n", name);
		fatal("can't create a module");
	}
	d = getmoduledict(m);
	for (ml = methods; ml->ml_name != NULL; ml++) {
		sprintf(namebuf, "%s.%s", name, ml->ml_name);
		v = newmethodobject(strdup(namebuf), ml->ml_meth, (object *)NULL);
		/* XXX The strdup'ed memory is never freed */
		if (v == NULL || dictinsert(d, ml->ml_name, v) != 0) {
			fprintf(stderr, "initializing module: %s\n", name);
			fatal("can't initialize module");
		}
		DECREF(v);
	}
	return m;
}

/* Argument list handling tools.
   All return 1 for success, or call err_set*() and return 0 for failure */

int
getnoarg(object *v)
{
	if (v != NULL) {
		return err_badarg();
	}
	return 1;
}

int
getintarg(object *v, int *a)
{
	if (v == NULL || !is_intobject(v)) {
		return err_badarg();
	}
	*a = getintvalue(v);
	return 1;
}

int
getintintarg(object *v, int *a, int *b)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getintarg(gettupleitem(v, 0), a)
           && getintarg(gettupleitem(v, 1), b);
}

int
getlongarg(object *v, long *a)
{
	if (v == NULL || !is_intobject(v)) {
		return err_badarg();
	}
	*a = getintvalue(v);
	return 1;
}

int
getlonglongargs(object *v, long *a, long *b)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getlongarg(gettupleitem(v, 0), a)
           && getlongarg(gettupleitem(v, 1), b);
}

int
getlonglongobjectargs(object *v, long *a, long *b, object **c)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 3) {
		return err_badarg();
	}
	if (getlongarg(gettupleitem(v, 0), a) && getlongarg(gettupleitem(v, 1), b))
    {
		*c = gettupleitem(v, 2);
		return 1;
	}
	else {
		return err_badarg();
	}
}

int
getstrarg(object *v, object **a)
{
	if (v == NULL || !is_stringobject(v)) {
		return err_badarg();
	}
	*a = v;
	return 1;
}

int
getstrstrarg(object *v, object **a, object **b)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getstrarg(gettupleitem(v, 0), a) &&
           getstrarg(gettupleitem(v, 1), b);
}

int
getstrstrintarg(object *v, object **a, object **b, int *c)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 3) {
		return err_badarg();
	}
	return getstrarg(gettupleitem(v, 0), a)
           && getstrarg(gettupleitem(v, 1), b)
           && getintarg(gettupleitem(v, 2), c);
}

int
getstrintarg(object *v, object **a, int *b)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getstrarg(gettupleitem(v, 0), a)
           && getintarg(gettupleitem(v, 1), b);
}

int
getintstrarg(object *v, int *a, object **b)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getintarg(gettupleitem(v, 0), a)
           && getstrarg(gettupleitem(v, 1), b);
}

/* int *a: [2] */
int
getpointarg(object *v, int *a)
{
	return getintintarg(v, a, a + 1);
}

/* int *a: [6] */
int
get3pointarg(object *v, int *a)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 3) {
		return err_badarg();
	}
	return getpointarg(gettupleitem(v, 0), a)
           && getpointarg(gettupleitem(v, 1), a + 2)
           && getpointarg(gettupleitem(v, 2), a + 4);
}

/* int *a: [2 + 2] */
int
getrectarg(object *v, int *a)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getpointarg(gettupleitem(v, 0), a)
           && getpointarg(gettupleitem(v, 1), a + 2);
}

/* int *a: [4 + 1] */
int
getrectintarg(object *v, int *a)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getrectarg(gettupleitem(v, 0), a)
           && getintarg(gettupleitem(v, 1), a + 4);
}

/*int *a; [2 + 1] */
int
getpointintarg(object *v, int *a)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getpointarg(gettupleitem(v, 0), a) &&
		   getintarg(gettupleitem(v, 1), a + 2);
}

/*int *a: [2] */
int
getpointstrarg(object *v, int *a, object **b)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getpointarg(gettupleitem(v, 0), a) &&
		   getstrarg(gettupleitem(v, 1), b);
}

int
getstrintintarg(object *v, object *a, int *b, int *c)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 3) {
		return err_badarg();
	}
	return getstrarg(gettupleitem(v, 0), &a)
           && getintarg(gettupleitem(v, 1), b)
           && getintarg(gettupleitem(v, 2), c);
}

/* int *a: [4 + 2] */
int
getrectpointarg(object *v, int *a)
{
	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		return err_badarg();
	}
	return getrectarg(gettupleitem(v, 0), a)
           && getpointarg(gettupleitem(v, 1), a + 4);
}

/* long *a: [n] */
int
getlongtuplearg(object *args, long *a, int n)
{
	if (!is_tupleobject(args) || gettuplesize(args) != n) {
		return err_badarg();
	}
	for (int i = 0; i < n; i++) {
		object *v = gettupleitem(args, i);
		if (!is_intobject(v)) {
			return err_badarg();
		}
		a[i] = getintvalue(v);
	}
	return 1;
}

/* short *a: [n] */
int
getshorttuplearg(object *args, short *a, int n)
{
	if (!is_tupleobject(args) || gettuplesize(args) != n) {
		return err_badarg();
	}
	for (int i = 0; i < n; i++) {
		object *v = gettupleitem(args, i);
		if (!is_intobject(v)) {
			return err_badarg();
		}
		a[i] = getintvalue(v);
	}
	return 1;
}

/*long *a: [n] */
int
getlonglistarg(object *args, long *a, int n)
{
	if (!is_listobject(args) || getlistsize(args) != n) {
		return err_badarg();
	}
	for (int i = 0; i < n; i++) {
		object *v = getlistitem(args, i);
		if (!is_intobject(v)) {
			return err_badarg();
		}
		a[i] = getintvalue(v);
	}
	return 1;
}

/* short *a: [n] */
int
getshortlistarg(object *args, short *a, int n)
{
	if (!is_listobject(args) || getlistsize(args) != n) {
		return err_badarg();
	}
	for (int i = 0; i < n; i++) {
		object *v = getlistitem(args, i);
		if (!is_intobject(v)) {
			return err_badarg();
		}
		a[i] = getintvalue(v);
	}
	return 1;
}
