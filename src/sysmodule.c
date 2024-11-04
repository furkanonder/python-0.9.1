/* System module */

/* Various bits of information used by the interpreter are collected in
module 'sys'.
Function member:
	- exit(sts): call (C, POSIX) exit(sts)
Data members:
	- stdin, stdout, stderr: standard file objects
	- modules: the table of modules (dictionary)
	- path: module search path (list of strings)
	- argv: script arguments (list of strings)
	- ps1, ps2: optional primary and secondary prompts (strings) */

#include <string.h>

#include "object.h"
#include "stringobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "methodobject.h"
#include "moduleobject.h"
#include "fileobject.h"
#include "errors.h"
#include "malloc.h"
#include "import.h"
#include "modsupport.h"
#include "pythonrun.h"

/* Define delimiter used in $PYTHONPATH */
#define DELIM ':'

static object *sysdict;

object *
sysget(char *name)
{
	return dictlookup(sysdict, name);
}

FILE *
sysgetfile(char *name, FILE *def)
{
	FILE *fp = NULL;
	object *v = sysget(name);

	if (v != NULL) {
		fp = getfilefile(v);
    }
	if (fp == NULL) {
		fp = def;
    }
	return fp;
}

int
sysset(char *name, object *v)
{
	if (v == NULL) {
		return dictremove(sysdict, name);
    }
	else {
		return dictinsert(sysdict, name, v);
    }
}

static object *
sys_exit(object *self, object *args)
{
	int sts;

	if (!getintarg(args, &sts)) {
		return NULL;
    }
	goaway(sts);
	exit(sts); /* Just in case */
	/* NOTREACHED */
}

static struct methodlist sys_methods[] = {
	{"exit",	sys_exit},
	{NULL,		NULL}	/* sentinel */
};

static object *sysin, *sysout, *syserr;

void
initsys()
{
	object *m = initmodule("sys", sys_methods);
	sysdict = getmoduledict(m);

	INCREF(sysdict);
	/* NB keep an extra ref to the std files to avoid closing them when the
       user deletes them */
	/* XXX File objects should have a "don't close" flag instead */
	sysin = newopenfileobject(stdin, "<stdin>", "r");
	sysout = newopenfileobject(stdout, "<stdout>", "w");
	syserr = newopenfileobject(stderr, "<stderr>", "w");
	if (err_occurred()) {
		fatal("can't create sys.std* file objects");
    }
	dictinsert(sysdict, "stdin", sysin);
	dictinsert(sysdict, "stdout", sysout);
	dictinsert(sysdict, "stderr", syserr);
	dictinsert(sysdict, "modules", get_modules());
	if (err_occurred()) {
		fatal("can't insert sys.* objects in sys dict");
    }
}

static object *
makepathobject(char *path, int delim)
{
	int i, n = 1;
	char *p = path;
	object *v, *w;

	while ((p = strchr(p, delim)) != NULL) {
		n++;
		p++;
	}
	v = newlistobject(n);
	if (v == NULL) {
		return NULL;
    }
	for (i = 0; ; i++) {
		p = strchr(path, delim);
		if (p == NULL) {
			p = strchr(path, '\0');	/* End of string */
        }
		w = newsizedstringobject(path, (int)(p - path));
		if (w == NULL) {
			DECREF(v);
			return NULL;
		}
		setlistitem(v, i, w);
		if (*p == '\0') {
			break;
        }
		path = p + 1;
	}
	return v;
}

void
setpythonpath(char *path)
{
	object *v;

	if ((v = makepathobject(path, DELIM)) == NULL) {
		fatal("can't create sys.path");
    }
	if (sysset("path", v) != 0) {
		fatal("can't assign sys.path");
    }
	DECREF(v);
}

static object *
makeargvobject(int argc, char **argv)
{
	object *av;

	if (argc < 0 || argv == NULL) {
		argc = 0;
    }
	av = newlistobject(argc);
	if (av != NULL) {
		for (int i = 0; i < argc; i++) {
			object *v = newstringobject(argv[i]);
			if (v == NULL) {
				DECREF(av);
				av = NULL;
				break;
			}
			setlistitem(av, i, v);
		}
	}
	return av;
}

void
setpythonargv(int argc, char **argv)
{
	object *av = makeargvobject(argc, argv);

	if (av == NULL) {
		fatal("no mem for sys.argv");
	}
	if (sysset("argv", av) != 0) {
		fatal("can't assign sys.argv");
    }
	DECREF(av);
}
