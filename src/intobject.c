/* Integer object implementation */

#include "allobjects.h"

/* Standard Booleans */

intobject FalseObject = {
	OB_HEAD_INIT(&Inttype)
	0
};

intobject TrueObject = {
	OB_HEAD_INIT(&Inttype)
	1
};

static object *
err_ovf()
{
	err_setstr(OverflowError, "integer overflow");
	return NULL;
}

static object *
err_zdiv()
{
	err_setstr(ZeroDivisionError, "integer division by zero");
	return NULL;
}

/* Integers are quite normal objects, to make object handling uniform.
   (Using odd pointers to represent integers would save much space
   but require extra checks for this special case throughout the code.)
   Since, a typical Python program spends much of its time allocating
   and deallocating integers, these operations should be very fast.
   Therefore we use a dedicated allocation scheme with a much lower
   overhead (in space and time) than straight malloc(): a simple
   dedicated free list, filled when necessary with memory from malloc().
*/

#define BLOCK_SIZE		1000	/* 1K less typical malloc overhead */
#define N_INTOBJECTS	(BLOCK_SIZE / sizeof(intobject))

static intobject *
fill_free_list()
{
	intobject *p = NEW(intobject, N_INTOBJECTS), *q;

	if (p == NULL) {
		return (intobject *)err_nomem();
    }
	q = p + N_INTOBJECTS;
	while (--q > p) {
		*(intobject **)q = q - 1;
    }
	*(intobject **)q = NULL;
	return p + N_INTOBJECTS - 1;
}

static intobject *free_list = NULL;

object *
newintobject(long ival)
{
	register intobject *v;

	if (free_list == NULL) {
		if ((free_list = fill_free_list()) == NULL) {
			return NULL;
        }
	}
	v = free_list;
	free_list = *(intobject **)free_list;
	NEWREF(v);
	v->ob_type = &Inttype;
	v->ob_ival = ival;
	return (object *) v;
}

static void
int_dealloc(intobject *v)
{
	*(intobject **)v = free_list;
	free_list = v;
}

long
getintvalue(register object *op)
{
	if (!is_intobject(op)) {
		err_badcall();
		return -1;
	}
	else {
		return ((intobject *)op)->ob_ival;
    }
}

/* Methods */

static void
int_print(intobject *v, FILE *fp, int flags)
{
	fprintf(fp, "%ld", v->ob_ival);
}

static object *
int_repr(intobject *v)
{
	char buf[20];

	sprintf(buf, "%ld", v->ob_ival);
	return newstringobject(buf);
}

static int
int_compare(intobject *v, intobject *w)
{
	register long i = v->ob_ival;
	register long j = w->ob_ival;

	return (i < j) ? -1 : (i > j) ? 1 : 0;
}

static object *
int_add(intobject *v, register object *w)
{
	register long a, b, x;

	if (!is_intobject(w)) {
		err_badarg();
		return NULL;
	}
	a = v->ob_ival;
	b = ((intobject *)w)->ob_ival;
	x = a + b;
	if ((x ^ a) < 0 && (x ^ b) < 0) {
		return err_ovf();
    }
	return newintobject(x);
}

static object *
int_sub(intobject *v, register object *w)
{
	register long a, b, x;

	if (!is_intobject(w)) {
		err_badarg();
		return NULL;
	}
	a = v->ob_ival;
	b = ((intobject *)w)->ob_ival;
	x = a - b;
	if ((x ^ a) < 0 && (x ^~ b) < 0) {
		return err_ovf();
    }
	return newintobject(x);
}

static object *
int_mul(intobject *v, register object *w)
{
	register long a, b;
	double x;

	if (!is_intobject(w)) {
		err_badarg();
		return NULL;
	}
	a = v->ob_ival;
	b = ((intobject *)w) -> ob_ival;
	x = (double)a * (double)b;
	if (x > 0x7fffffff || x < (double)(long)0x80000000) {
		return err_ovf();
    }
	return newintobject(a * b);
}

static object *
int_div(intobject *v, register object *w)
{
	if (!is_intobject(w)) {
		err_badarg();
		return NULL;
	}
	if (((intobject *)w)->ob_ival == 0) {
		return err_zdiv();
    }
	return newintobject(v->ob_ival / ((intobject *)w)->ob_ival);
}

static object *
int_rem(intobject *v, register object *w)
{
	if (!is_intobject(w)) {
		err_badarg();
		return NULL;
	}
	if (((intobject *)w)->ob_ival == 0) {
		return err_zdiv();
    }
	return newintobject(v->ob_ival % ((intobject *)w)->ob_ival);
}

static object *
int_pow(intobject *v, register object *w)
{
	register long iv, iw, ix;
	register int neg;

	if (!is_intobject(w)) {
		err_badarg();
		return NULL;
	}
	iv = v->ob_ival;
	iw = ((intobject *)w)->ob_ival;
	neg = 0;
	if (iw < 0) {
		neg = 1, iw = -iw;
    }
	ix = 1;
	for (; iw > 0; iw--) {
		ix = ix * iv;
    }
	if (neg) {
		if (ix == 0) {
			return err_zdiv();
        }
		ix = 1 / ix;
	}
	/* XXX How to check for overflow? */
	return newintobject(ix);
}

static object *
int_neg(intobject *v)
{
	register long a = v->ob_ival, x;
	x = -a;

	if (a < 0 && x < 0) {
		return err_ovf();
    }
	return newintobject(x);
}

static object *
int_pos(intobject *v)
{
	INCREF(v);
	return (object *)v;
}

static number_methods int_as_number = {
	(binaryfunc)int_add,	/*tp_add*/
	(binaryfunc)int_sub,	/*tp_subtract*/
	(binaryfunc)int_mul,	/*tp_multiply*/
	(binaryfunc)int_div,	/*tp_divide*/
	(binaryfunc)int_rem,	/*tp_remainder*/
	(ternaryfunc)int_pow,	/*tp_power*/
	(unaryfunc)int_neg,		/*tp_negate*/
	(unaryfunc)int_pos,		/*tp_plus*/
};

typeobject Inttype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"int",
	sizeof(intobject),
	0,
	(destructor)int_dealloc,	/*tp_dealloc*/
	(printfunc)int_print,		/*tp_print*/
	0,							/*tp_getattr*/
	0,							/*tp_setattr*/
	(cmpfunc)int_compare,		/*tp_compare*/
	(reprfunc)int_repr,			/*tp_repr*/
	&int_as_number,				/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
};
