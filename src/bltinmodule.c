/* Built-in functions */

#include <stdio.h>
#include <string.h>

#include "object.h"
#include "intobject.h"
#include "floatobject.h"
#include "tupleobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "methodobject.h"
#include "moduleobject.h"
#include "fileobject.h"
#include "errors.h"
#include "node.h"
#include "graminit.h"
#include "errcode.h"
#include "sysmodule.h"
#include "import.h"
#include "pythonrun.h"
#include "compile.h"	/* For ceval.h */
#include "ceval.h"
#include "modsupport.h"
#include "fgetsintr.h"

static object *builtin_dict;

static object *
builtin_abs(object *self, object *v)
{
	/* XXX This should be a method in the as_number struct in the type */
	if (v == NULL) {
		/* */
	}
	else if (is_intobject(v)) {
		long x = getintvalue(v);
		if (x < 0) {
			x = -x;
        }
		return newintobject(x);
	}
	else if (is_floatobject(v)) {
		double x = getfloatvalue(v);
		if (x < 0) {
			x = -x;
        }
		return newfloatobject(x);
	}
	err_setstr(TypeError, "abs() argument must be float or int");
	return NULL;
}

static object *
builtin_chr(object *self, object *v)
{
	long x;
	char s[1];

	if (v == NULL || !is_intobject(v)) {
		err_setstr(TypeError, "chr() must have int argument");
		return NULL;
	}
	x = getintvalue(v);
	if (x < 0 || x >= 256) {
		err_setstr(RuntimeError, "chr() arg not in range(256)");
		return NULL;
	}
	s[0] = x;
	return newsizedstringobject(s, 1);
}

static object *
builtin_dir(object *self, object *v)
{
	object *d;

	if (v == NULL) {
		d = getlocals();
	}
	else {
		if (!is_moduleobject(v)) {
			err_setstr(TypeError, "dir() argument, must be module or absent");
			return NULL;
		}
		d = getmoduledict(v);
	}
	v = getdictkeys(d);
	if (sortlist(v) != 0) {
		DECREF(v);
		v = NULL;
	}
	return v;
}

static object *
builtin_divmod(object *self, object *v)
{
	object *x, *y;
	long xi, yi, xdivy, xmody;

	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2) {
		err_setstr(TypeError, "divmod() requires 2 int arguments");
		return NULL;
	}
	x = gettupleitem(v, 0);
	y = gettupleitem(v, 1);
	if (!is_intobject(x) || !is_intobject(y)) {
		err_setstr(TypeError, "divmod() requires 2 int arguments");
		return NULL;
	}
	xi = getintvalue(x);
	yi = getintvalue(y);
	if (yi == 0) {
		err_setstr(TypeError, "divmod() division by zero");
		return NULL;
	}
	if (yi < 0) {
		xdivy = -xi / -yi;
	}
	else {
		xdivy = xi / yi;
	}
	xmody = xi - xdivy * yi;
	if ((xmody < 0 && yi > 0) || (xmody > 0 && yi < 0)) {
		xmody += yi;
		xdivy -= 1;
	}
	v = newtupleobject(2);
	x = newintobject(xdivy);
	y = newintobject(xmody);
	if (v == NULL || x == NULL || y == NULL || settupleitem(v, 0, x) != 0
        || settupleitem(v, 1, y) != 0)
    {
		XDECREF(v);
		XDECREF(x);
		XDECREF(y);
		return NULL;
	}
	return v;
}

static object *
exec_eval(object *v, int start)
{
	object *str = NULL, *globals = NULL, *locals = NULL;
	int n;

	if (v != NULL) {
		if (is_stringobject(v)) {
			str = v;
        }
		else if (is_tupleobject(v) && ((n = gettuplesize(v)) == 2 || n == 3)) {
			str = gettupleitem(v, 0);
			globals = gettupleitem(v, 1);
			if (n == 3) {
				locals = gettupleitem(v, 2);
            }
		}
	}
	if (str == NULL || !is_stringobject(str)
		|| (globals != NULL && !is_dictobject(globals))
        || (locals != NULL && !is_dictobject(locals)))
    {
		err_setstr(TypeError,
                   "exec/eval arguments must be string[,dict[,dict]]");
		return NULL;
	}
	return run_string(getstringvalue(str), start, globals, locals);
}

static object *
builtin_eval(object *self, object *v)
{
	return exec_eval(v, eval_input);
}

static object *
builtin_exec(object *self, object *v)
{
	return exec_eval(v, file_input);
}

static object *
builtin_float(object *self, object *v)
{
	if (v == NULL) {
		/* */
	}
	else if (is_floatobject(v)) {
		INCREF(v);
		return v;
	}
	else if (is_intobject(v)) {
		long x = getintvalue(v);
		return newfloatobject((double)x);
	}
	err_setstr(TypeError, "float() argument must be float or int");
	return NULL;
}

static object *
builtin_input(object *self, object *v)
{
	FILE *in = sysgetfile("stdin", stdin);
	FILE *out = sysgetfile("stdout", stdout);
	object *m, *d;

	flushline();
	if (v != NULL) {
		printobject(v, out, PRINT_RAW);
    }
	m = add_module("__main__");
	d = getmoduledict(m);
	return run_file(in, "<stdin>", expr_input, d, d);
}

static object *
builtin_int(object *self, object *v)
{
	if (v == NULL) {
		/* */
	}
	else if (is_intobject(v)) {
		INCREF(v);
		return v;
	}
	else if (is_floatobject(v)) {
		double x = getfloatvalue(v);
		return newintobject((long)x);
	}
	err_setstr(TypeError, "int() argument must be float or int");
	return NULL;
}

static object *
builtin_len(object *self, object *v)
{
	long len;
	typeobject *tp;

	if (v == NULL) {
		err_setstr(TypeError, "len() without argument");
		return NULL;
	}
	tp = v->ob_type;
	if (tp->tp_as_sequence != NULL) {
		len = (*tp->tp_as_sequence->sq_length)(v);
	}
	else if (tp->tp_as_mapping != NULL) {
		len = (*tp->tp_as_mapping->mp_length)(v);
	}
	else {
		err_setstr(TypeError, "len() of unsized object");
		return NULL;
	}
	return newintobject(len);
}

static object *
min_max(object *v, int sign)
{
	int i, n, cmp;
	object *w, *x;
	sequence_methods *sq;

	if (v == NULL) {
		err_setstr(TypeError, "min() or max() without argument");
		return NULL;
	}
	sq = v->ob_type->tp_as_sequence;
	if (sq == NULL) {
		err_setstr(TypeError, "min() or max() of non-sequence");
		return NULL;
	}
	n = (*sq->sq_length)(v);
	if (n == 0) {
		err_setstr(RuntimeError, "min() or max() of empty sequence");
		return NULL;
	}
	w = (*sq->sq_item)(v, 0); /* Implies INCREF */
	for (i = 1; i < n; i++) {
		x = (*sq->sq_item)(v, i); /* Implies INCREF */
		cmp = cmpobject(x, w);
		if (cmp * sign > 0) {
			DECREF(w);
			w = x;
		}
		else {
			DECREF(x);
        }
	}
	return w;
}

static object *
builtin_min(object *self, object *v)
{
	return min_max(v, -1);
}

static object *
builtin_max(object *self, object *v)
{
	return min_max(v, 1);
}

static object *
builtin_open(object *self, object *v)
{
	object *name, *mode;

	if (v == NULL || !is_tupleobject(v) || gettuplesize(v) != 2
        || !is_stringobject(name = gettupleitem(v, 0))
        || !is_stringobject(mode = gettupleitem(v, 1)))
    {
		err_setstr(TypeError, "open() requires 2 string arguments");
		return NULL;
	}
	v = newfileobject(getstringvalue(name), getstringvalue(mode));
	return v;
}

static object *
builtin_ord(object *self, object *v)
{
	if (v == NULL || !is_stringobject(v)) {
		err_setstr(TypeError, "ord() must have string argument");
		return NULL;
	}
	if (getstringsize(v) != 1) {
		err_setstr(RuntimeError, "ord() arg must have length 1");
		return NULL;
	}
	return newintobject((long)(getstringvalue(v)[0] & 0xff));
}

static object *
builtin_range(object *self, object *v)
{
	static char *errmsg = "range() requires 1-3 int arguments";
	int i, n;
	long ilow, ihigh, istep;

	if (v != NULL && is_intobject(v)) {
		ilow = 0;
        ihigh = getintvalue(v);
        istep = 1;
	}
	else if (v == NULL || !is_tupleobject(v)) {
		err_setstr(TypeError, errmsg);
		return NULL;
	}
	else {
		n = gettuplesize(v);
		if (n < 1 || n > 3) {
			err_setstr(TypeError, errmsg);
			return NULL;
		}
		for (i = 0; i < n; i++) {
			if (!is_intobject(gettupleitem(v, i))) {
				err_setstr(TypeError, errmsg);
				return NULL;
			}
		}
		if (n == 3) {
			istep = getintvalue(gettupleitem(v, 2));
			--n;
		}
		else {
			istep = 1;
        }
		ihigh = getintvalue(gettupleitem(v, --n));
		if (n > 0) {
			ilow = getintvalue(gettupleitem(v, 0));
        }
		else {
			ilow = 0;
        }
	}
	if (istep == 0) {
		err_setstr(RuntimeError, "zero step for range()");
		return NULL;
	}
	/* XXX ought to check overflow of subtraction */
	if (istep > 0) {
		n = (ihigh - ilow + istep - 1) / istep;
    }
	else {
		n = (ihigh - ilow + istep + 1) / istep;
    }
	if (n < 0) {
		n = 0;
    }
	v = newlistobject(n);
	if (v == NULL) {
		return NULL;
    }
	for (i = 0; i < n; i++) {
		object *w = newintobject(ilow);
		if (w == NULL) {
			DECREF(v);
			return NULL;
		}
		setlistitem(v, i, w);
		ilow += istep;
	}
	return v;
}

static object *
builtin_raw_input(object *self, object *v)
{
	FILE *in = sysgetfile("stdin", stdin);
	FILE *out = sysgetfile("stdout", stdout);
	int err;
	int n = 1000;

	flushline();
	if (v != NULL) {
		printobject(v, out, PRINT_RAW);
    }
	v = newsizedstringobject((char *)NULL, n);
	if (v != NULL) {
		if ((err = fgets_intr(getstringvalue(v), n + 1, in)) != E_OK) {
			err_input(err);
			DECREF(v);
			return NULL;
		}
		else {
			n = strlen(getstringvalue(v));
			if (n > 0 && getstringvalue(v)[n - 1] == '\n') {
				n--;
            }
			resizestring(&v, n);
		}
	}
	return v;
}

static object *
builtin_reload(object *self, object *v)
{
	return reload_module(v);
}

static object *
builtin_type(object *self, object *v)
{
	if (v == NULL) {
		err_setstr(TypeError, "type() requres an argument");
		return NULL;
	}
	v = (object *)v->ob_type;
	INCREF(v);
	return v;
}

static struct methodlist builtin_methods[] = {
	{"abs", 		builtin_abs},
	{"chr", 		builtin_chr},
	{"dir", 		builtin_dir},
	{"divmod", 		builtin_divmod},
	{"eval", 		builtin_eval},
	{"exec", 		builtin_exec},
	{"float", 		builtin_float},
	{"input", 		builtin_input},
	{"int",		    builtin_int},
	{"len", 		builtin_len},
	{"max", 		builtin_max},
	{"min", 		builtin_min},
	{"open", 		builtin_open}, /* XXX move to OS module */
	{"ord", 		builtin_ord},
	{"range", 		builtin_range},
	{"raw_input",	builtin_raw_input},
	{"reload", 		builtin_reload},
	{"type", 		builtin_type},
	{NULL, 			NULL},
};

object *
getbuiltin(char *name)
{
	return dictlookup(builtin_dict, name);
}

/* Predefined exceptions */
object *RuntimeError;
object *EOFError;
object *TypeError;
object *MemoryError;
object *NameError;
object *SystemError;
object *KeyboardInterrupt;

static object *
newstdexception(char *name, char *message)
{
	object *v = newstringobject(message);

	if (v == NULL || dictinsert(builtin_dict, name, v) != 0) {
		fatal("no mem for new standard exception");
    }
	return v;
}

static void
initerrors()
{
	RuntimeError 		= 	newstdexception("RuntimeError", "run-time error");
	EOFError 			= 	newstdexception("EOFError", "end-of-file read");
	TypeError 			= 	newstdexception("TypeError", "type error");
	MemoryError 		= 	newstdexception("MemoryError", "out of memory");
	NameError 			= 	newstdexception("NameError", "undefined name");
	SystemError 		= 	newstdexception("SystemError", "system error");
	KeyboardInterrupt	= 	newstdexception("KeyboardInterrupt",
                                        	"keyboard interrupt");
}

void
initbuiltin()
{
	object *m = initmodule("builtin", builtin_methods);
	builtin_dict = getmoduledict(m);

	INCREF(builtin_dict);
	initerrors();
	(void)dictinsert(builtin_dict, "None", None);
}
