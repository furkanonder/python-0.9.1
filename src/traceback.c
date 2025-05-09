/* Traceback implementation */

#include <string.h>
#include <stddef.h>

#include "object.h"
#include "objimpl.h"
#include "errors.h"
#include "malloc.h"
#include "compile.h"
#include "frameobject.h"
#include "structmember.h"
#include "intrcheck.h"
#include "sysmodule.h"

typedef struct _tracebackobject {
	OB_HEAD
	struct _tracebackobject *tb_next;
	frameobject 			*tb_frame;
	int 					tb_lasti;
	int 					tb_lineno;
} tracebackobject;

#define OFF(x) offsetof(tracebackobject, x)

static struct memberlist tb_memberlist[] = {
	{"tb_next",		T_OBJECT,	OFF(tb_next)},
	{"tb_frame",	T_OBJECT,	OFF(tb_frame)},
	{"tb_lasti",	T_INT,		OFF(tb_lasti)},
	{"tb_lineno",	T_INT,		OFF(tb_lineno)},
	{NULL}	/* Sentinel */
};

static object *
tb_getattr(tracebackobject *tb, char *name)
{
	return getmember((char *)tb, tb_memberlist, name);
}

static void
tb_dealloc(tracebackobject *tb)
{
	XDECREF(tb->tb_next);
	XDECREF(tb->tb_frame);
	DEL(tb);
}

static typeobject Tracebacktype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"traceback",
	sizeof(tracebackobject),
	0,
	(destructor)tb_dealloc,		/*tp_dealloc*/
	0,							/*tp_print*/
	(getattrfunc)tb_getattr,	/*tp_getattr*/
	0,							/*tp_setattr*/
	0,							/*tp_compare*/
	0,							/*tp_repr*/
	0,							/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
};

#define is_tracebackobject(v) ((v)->ob_type == &Tracebacktype)

static tracebackobject *
newtracebackobject(tracebackobject *next, frameobject *frame, int lasti,
                   int lineno)
{
	tracebackobject *tb;

	if ((next != NULL && !is_tracebackobject(next)) || frame == NULL
         || !is_frameobject(frame))
    {
		err_badcall();
		return NULL;
	}
	tb = NEWOBJ(tracebackobject, &Tracebacktype);
	if (tb != NULL) {
		XINCREF(next);
		tb->tb_next = next;
		XINCREF(frame);
		tb->tb_frame = frame;
		tb->tb_lasti = lasti;
		tb->tb_lineno = lineno;
	}
	return tb;
}

static tracebackobject *tb_current = NULL;

int
tb_here(frameobject *frame, int lasti, int lineno)
{
	tracebackobject *tb = newtracebackobject(tb_current, frame, lasti, lineno);

	if (tb == NULL) {
		return -1;
    }
	XDECREF(tb_current);
	tb_current = tb;
	return 0;
}

object *
tb_fetch()
{
	object *v = (object *)tb_current;

	tb_current = NULL;
	return v;
}

int
tb_store(object *v)
{
	if (v != NULL && !is_tracebackobject(v)) {
		err_badcall();
		return -1;
	}
	XDECREF(tb_current);
	XINCREF(v);
	tb_current = (tracebackobject *)v;
	return 0;
}

static void
tb_displayline(FILE *fp, char *filename, int lineno)
{
	FILE *xfp;
	char buf[1000];
	int i;

	if (filename[0] == '<' && filename[strlen(filename)-1] == '>') {
		return;
    }
	xfp = fopen(filename, "r");
	if (xfp == NULL) {
		fprintf(fp, "    (cannot open \"%s\")\n", filename);
		return;
	}
	for (i = 0; i < lineno; i++) {
		if (fgets(buf, sizeof buf, xfp) == NULL) {
			break;
        }
	}
	if (i == lineno) {
		char *p = buf;
		while (*p == ' ' || *p == '\t') {
			p++;
        }
		fprintf(fp, "    %s", p);
		if (strchr(p, '\n') == NULL) {
			fprintf(fp, "\n");
        }
	}
	fclose(xfp);
}

static void
tb_printinternal(tracebackobject *tb, FILE *fp)
{
	while (tb != NULL) {
		if (intrcheck()) {
			fprintf(fp, "[interrupted]\n");
			break;
		}
		fprintf(fp, "  File \"");
		printobject(tb->tb_frame->f_code->co_filename, fp, PRINT_RAW);
		fprintf(fp, "\", line %d\n", tb->tb_lineno);
		tb_displayline(fp, getstringvalue(tb->tb_frame->f_code->co_filename),
					   tb->tb_lineno);
		tb = tb->tb_next;
	}
}

int
tb_print(object *v, FILE *fp)
{
	if (v == NULL) {
		return 0;
    }
	if (!is_tracebackobject(v)) {
		err_badcall();
		return -1;
	}
	sysset("last_traceback", v);
	tb_printinternal((tracebackobject *)v, fp);
	return 0;
}
