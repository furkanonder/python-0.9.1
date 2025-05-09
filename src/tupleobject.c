/* Tuple object implementation */

#include "object.h"
#include "objimpl.h"
#include "stringobject.h"
#include "tupleobject.h"
#include "errors.h"
#include "malloc.h"

object *
newtupleobject(register int size)
{
	register int i;
	register tupleobject *op;

	if (size < 0) {
		err_badcall();
		return NULL;
	}
	op = (tupleobject *)malloc(sizeof(tupleobject) + size * sizeof(object *));
	if (op == NULL) {
		return err_nomem();
    }
	NEWREF(op);
	op->ob_type = &Tupletype;
	op->ob_size = size;
	for (i = 0; i < size; i++) {
		op->ob_item[i] = NULL;
    }
	return (object *)op;
}

int
gettuplesize(register object *op)
{
	if (!is_tupleobject(op)) {
		err_badcall();
		return -1;
	}
	else {
		return ((tupleobject *)op)->ob_size;
    }
}

object *
gettupleitem(register object *op, register int i)
{
	if (!is_tupleobject(op)) {
		err_badcall();
		return NULL;
	}
	if (i < 0 || i >= ((tupleobject *)op)->ob_size) {
		err_setstr(IndexError, "tuple index out of range");
		return NULL;
	}
	return ((tupleobject *)op)->ob_item[i];
}

int
settupleitem(register object *op, register int i, register object *newitem)
{
	register object *olditem;

	if (!is_tupleobject(op)) {
		if (newitem != NULL) {
			DECREF(newitem);
        }
		err_badcall();
		return -1;
	}
	if (i < 0 || i >= ((tupleobject *)op)->ob_size) {
		if (newitem != NULL) {
			DECREF(newitem);
        }
		err_setstr(IndexError, "tuple assignment index out of range");
		return -1;
	}
	olditem = ((tupleobject *)op)->ob_item[i];
	((tupleobject *)op)->ob_item[i] = newitem;
	if (olditem != NULL) {
		DECREF(olditem);
    }
	return 0;
}

/* Methods */

static void
tupledealloc(register tupleobject *op)
{
	register int i;

	for (i = 0; i < op->ob_size; i++) {
		if (op->ob_item[i] != NULL) {
			DECREF(op->ob_item[i]);
        }
	}
	free((ANY *)op);
}

static void
tupleprint(tupleobject *op, FILE *fp, int flags)
{
	fprintf(fp, "(");
	for (int i = 0; i < op->ob_size && !StopPrint; i++) {
		if (i > 0) {
			fprintf(fp, ", ");
		}
		printobject(op->ob_item[i], fp, flags);
	}
	if (op->ob_size == 1) {
		fprintf(fp, ",");
    }
	fprintf(fp, ")");
}

object *
tuplerepr(tupleobject *v)
{
	object *s = newstringobject("("), *t, *comma = newstringobject(", ");

	for (int i = 0; i < v->ob_size && s != NULL; i++) {
		if (i > 0) {
			joinstring(&s, comma);
        }
		t = reprobject(v->ob_item[i]);
		joinstring(&s, t);
		if (t != NULL) {
			DECREF(t);
        }
	}
	DECREF(comma);
	if (v->ob_size == 1) {
		t = newstringobject(",");
		joinstring(&s, t);
		DECREF(t);
	}
	t = newstringobject(")");
	joinstring(&s, t);
	DECREF(t);
	return s;
}

static int
tuplecompare(register tupleobject *v, register tupleobject *w)
{
	register int len = (v->ob_size < w->ob_size) ? v->ob_size : w->ob_size;
	register int i;

	for (i = 0; i < len; i++) {
		int cmp = cmpobject(v->ob_item[i], w->ob_item[i]);
		if (cmp != 0) {
			return cmp;
        }
	}
	return v->ob_size - w->ob_size;
}

static int
tuplelength(tupleobject *a)
{
	return a->ob_size;
}

static object *
tupleitem(register tupleobject *a, register int i)
{
	if (i < 0 || i >= a->ob_size) {
		err_setstr(IndexError, "tuple index out of range");
		return NULL;
	}
	INCREF(a->ob_item[i]);
	return a->ob_item[i];
}

static object *
tupleslice(register tupleobject *a, register int ilow, register int ihigh)
{
	register tupleobject *np;
	register int i;

	if (ilow < 0) {
		ilow = 0;
    }
	if (ihigh > a->ob_size) {
		ihigh = a->ob_size;
    }
	if (ihigh < ilow) {
		ihigh = ilow;
    }
	if (ilow == 0 && ihigh == a->ob_size) {
		/* XXX can only do this if tuples are immutable! */
		INCREF(a);
		return (object *)a;
	}
	np = (tupleobject *)newtupleobject(ihigh - ilow);
	if (np == NULL) {
		return NULL;
    }
	for (i = ilow; i < ihigh; i++) {
		object *v = a->ob_item[i];
		INCREF(v);
		np->ob_item[i - ilow] = v;
	}
	return (object *)np;
}

static object *
tupleconcat(register tupleobject *a, register object *bb)
{
	register int size, i;
	tupleobject *np;

	if (!is_tupleobject(bb)) {
		err_badarg();
		return NULL;
	}
#define b ((tupleobject *)bb)
	size = a->ob_size + b->ob_size;
	np = (tupleobject *)newtupleobject(size);
	if (np == NULL) {
		return err_nomem();
	}
	for (i = 0; i < a->ob_size; i++) {
		object *v = a->ob_item[i];
		INCREF(v);
		np->ob_item[i] = v;
	}
	for (i = 0; i < b->ob_size; i++) {
		object *v = b->ob_item[i];
		INCREF(v);
		np->ob_item[i + a->ob_size] = v;
	}
	return (object *)np;
#undef b
}

static sequence_methods tuple_as_sequence = {
	(inquiry)tuplelength,		/*sq_length*/
	(binaryfunc)tupleconcat,	/*sq_concat*/
	0,							/*sq_repeat*/
	(intargfunc)tupleitem,		/*sq_item*/
	(intintargfunc)tupleslice,	/*sq_slice*/
	0,							/*sq_ass_item*/
	0,							/*sq_ass_slice*/
};

typeobject Tupletype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"tuple",
	sizeof(tupleobject) - sizeof(object *),
	sizeof(object *),
	(destructor)tupledealloc,	/*tp_dealloc*/
	(printfunc)tupleprint,		/*tp_print*/
	0,							/*tp_getattr*/
	0,							/*tp_setattr*/
	(cmpfunc)tuplecompare,		/*tp_compare*/
	(reprfunc)tuplerepr,		/*tp_repr*/
	0,							/*tp_as_number*/
	&tuple_as_sequence,			/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
};
