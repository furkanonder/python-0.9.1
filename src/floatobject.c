/* Float object implementation */

/* XXX There should be overflow checks here, but it's hard to check for any
   kind of float exception without losing portability. */

#include <ctype.h>
#include <math.h>
#include <errno.h>
#ifndef errno
extern int errno;
#endif

#include "object.h"
#include "floatobject.h"
#include "stringobject.h"
#include "errors.h"
#include "malloc.h"

object *
newfloatobject(double fval)
{
	/* For efficiency, this code is copied from newobject() */
	register floatobject *op = (floatobject *)malloc(sizeof(floatobject));

	if (op == NULL) {
		return err_nomem();
    }
	NEWREF(op);
	op->ob_type = &Floattype;
	op->ob_fval = fval;
	return (object *)op;
}

void
float_dealloc(floatobject *op)
{
	DEL(op);
}

double
getfloatvalue(object *op)
{
	if (!is_floatobject(op)) {
		err_badarg();
		return -1;
	}
	else {
		return ((floatobject *)op)->ob_fval;
    }
}

/* Methods */

static void
float_buf_repr(char *buf, floatobject *v)
{
	register char *cp;

	/* Subroutine for float_repr and float_print.  We want float numbers to be
       recognizable as such, i.e., they should contain a decimal point or an
       exponent. However, %g may print the number as an integer; in such cases,
       we append ".0" to the string. */
	sprintf(buf, "%.12g", v->ob_fval);
	cp = buf;
	if (*cp == '-') {
		cp++;
    }
	for (; *cp != '\0'; cp++) {
		/* Any non-digit means it's not an integer;
		   this takes care of NAN and INF as well. */
		if (!isdigit(*cp)) {
			break;
        }
	}
	if (*cp == '\0') {
		*cp++ = '.';
		*cp++ = '0';
		*cp++ = '\0';
	}
}

static void
float_print(floatobject *v, FILE *fp, int flags)
{
	char buf[100];

	float_buf_repr(buf, v);
	fputs(buf, fp);
}

static object *
float_repr(floatobject *v)
{
	char buf[100];

	float_buf_repr(buf, v);
	return newstringobject(buf);
}

static int
float_compare(floatobject *v, floatobject *w)
{
	double i = v->ob_fval;
	double j = w->ob_fval;

	return (i < j) ? -1 : (i > j) ? 1 : 0;
}

static object *
float_add(floatobject *v, object *w)
{
	if (!is_floatobject(w)) {
		err_badarg();
		return NULL;
	}
	return newfloatobject(v->ob_fval + ((floatobject *)w)->ob_fval);
}

static object *
float_sub(floatobject *v, object *w)
{
	if (!is_floatobject(w)) {
		err_badarg();
		return NULL;
	}
	return newfloatobject(v->ob_fval - ((floatobject *)w)->ob_fval);
}

static object *
float_mul(floatobject *v, object *w)
{
	if (!is_floatobject(w)) {
		err_badarg();
		return NULL;
	}
	return newfloatobject(v->ob_fval * ((floatobject *)w)->ob_fval);
}

static object *
float_div(floatobject *v, object *w)
{
	if (!is_floatobject(w)) {
		err_badarg();
		return NULL;
	}
	if (((floatobject *)w)->ob_fval == 0) {
		err_setstr(ZeroDivisionError, "float division by zero");
		return NULL;
	}
	return newfloatobject(v->ob_fval / ((floatobject *)w)->ob_fval);
}

static object *
float_rem(floatobject *v, object *w)
{
	double wx;

	if (!is_floatobject(w)) {
		err_badarg();
		return NULL;
	}
	wx = ((floatobject *)w)->ob_fval;
	if (wx == 0.0) {
		err_setstr(ZeroDivisionError, "float division by zero");
		return NULL;
	}
	return newfloatobject(fmod(v->ob_fval, wx));
}

static object *
float_pow(floatobject *v, object *w)
{
	double iv, iw, ix;

	if (!is_floatobject(w)) {
		err_badarg();
		return NULL;
	}
	iv = v->ob_fval;
	iw = ((floatobject *)w)->ob_fval;
	if (iw == 0.0) {
		return newfloatobject(1.0); /* x**0 is always 1, even 0**0 */
    }
	errno = 0;
	ix = pow(iv, iw);
	if (errno != 0) {
		/* XXX could it be another type of error? */
		err_errno(OverflowError);
		return NULL;
	}
	return newfloatobject(ix);
}

static object *
float_neg(floatobject *v)
{
	return newfloatobject(-v->ob_fval);
}

static object *
float_pos(floatobject *v)
{
	return newfloatobject(v->ob_fval);
}

static number_methods float_as_number = {
	(binaryfunc)float_add,	/*tp_add*/
	(binaryfunc)float_sub,	/*tp_subtract*/
	(binaryfunc)float_mul,	/*tp_multiply*/
	(binaryfunc)float_div,	/*tp_divide*/
	(binaryfunc)float_rem,	/*tp_remainder*/
	(ternaryfunc)float_pow,	/*tp_power*/
	(unaryfunc)float_neg,	/*tp_negate*/
	(unaryfunc)float_pos,	/*tp_plus*/
};

typeobject Floattype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"float",
	sizeof(floatobject),
	0,
	(destructor)float_dealloc,	/*tp_dealloc*/
	(printfunc)float_print,		/*tp_print*/
	0,							/*tp_getattr*/
	0,							/*tp_setattr*/
	(cmpfunc)float_compare,		/*tp_compare*/
	(reprfunc)float_repr,		/*tp_repr*/
	&float_as_number,			/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
};

/* XXX This is not enough.  Need:
- automatic casts for mixed arithmetic (3.1 * 4)
- mixed comparisons (!)
- look at other uses of ints that could be extended to floats */
