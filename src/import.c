/* Module definition and import implementation */

#include <string.h>

#include "object.h"
#include "stringobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "moduleobject.h"
#include "errors.h"
#include "node.h"
#include "graminit.h"
#include "errcode.h"
#include "sysmodule.h"
#include "pythonrun.h"
#include "modsupport.h"

/* Define pathname separator used in file names */
#define SEP '/'

static object *modules;
static int init_builtin(char *name);

void
initimport()
{
	if ((modules = newdictobject()) == NULL) {
		fatal("no mem for dictionary of modules");
    }
}

object *
get_modules()
{
	return modules;
}

object *
add_module(char *name)
{
	object *m;

	if ((m = dictlookup(modules, name)) != NULL && is_moduleobject(m)) {
		return m;
    }
	m = newmoduleobject(name);
	if (m == NULL) {
		return NULL;
    }
	if (dictinsert(modules, name, m) != 0) {
		DECREF(m);
		return NULL;
	}
	DECREF(m); /* Yes, it still exists, in modules! */
	return m;
}

static FILE *
open_module(char *name, char *suffix, char *namebuf)
{
	object *path = sysget("path");
	FILE *fp;

	if (path == NULL || !is_listobject(path)) {
		strcpy(namebuf, name);
		strcat(namebuf, suffix);
		fp = fopen(namebuf, "r");
	}
	else {
		fp = NULL;
		for (int i = 0; i < getlistsize(path); i++) {
			object *v = getlistitem(path, i);
			if (!is_stringobject(v)) {
				continue;
            }
			strcpy(namebuf, getstringvalue(v));
			int len = getstringsize(v);
			if (len > 0 && namebuf[len - 1] != SEP) {
				namebuf[len++] = SEP;
            }
			strcpy(namebuf + len, name);
			strcat(namebuf, suffix);
			fp = fopen(namebuf, "r");
			if (fp != NULL) {
				break;
            }
		}
	}
	return fp;
}

static object *
get_module(object *m, char *name, object **m_ret)
{
	object *d;
	node *n;
	int err;
	char namebuf[256];
	FILE *fp = open_module(name, ".py", namebuf);

	if (fp == NULL) {
		if (m == NULL) {
			err_setstr(NameError, name);
        }
		else {
			err_setstr(RuntimeError, "no module source file");
        }
		return NULL;
	}
	err = parse_file(fp, namebuf, file_input, &n);
	fclose(fp);
	if (err != E_DONE) {
		err_input(err);
		return NULL;
	}
	if (m == NULL) {
		m = add_module(name);
		if (m == NULL) {
			freetree(n);
			return NULL;
		}
		*m_ret = m;
	}
	d = getmoduledict(m);
	return run_node(n, namebuf, d, d);
}

static object *
load_module(char *name)
{
	object *m, *v = get_module((object *)NULL, name, &m);

	if (v == NULL) {
		return NULL;
    }
	DECREF(v);
	return m;
}

object *
import_module(char *name)
{
	object *m;

	if ((m = dictlookup(modules, name)) == NULL) {
		if (init_builtin(name)) {
			if ((m = dictlookup(modules, name)) == NULL) {
				err_setstr(SystemError, "builtin module missing");
            }
		}
		else {
			m = load_module(name);
		}
	}
	return m;
}

object *
reload_module(object *m)
{
	if (m == NULL || !is_moduleobject(m)) {
		err_setstr(TypeError, "reload() argument must be module");
		return NULL;
	}
	/* XXX Ought to check for builtin modules -- can't reload these... */
	return get_module(m, getmodulename(m), (object **)NULL);
}

static void
cleardict(object *d)
{
	for (int i = getdictsize(d); --i >= 0; ) {
		char *k = getdictkey(d, i);
		if (k != NULL) {
			(void) dictremove(d, k);
        }
	}
}

void
doneimport()
{
	if (modules != NULL) {
		/* Explicitly erase all modules; this is the safest way
		   to get rid of at least *some* circular dependencies */
		for (int i = getdictsize(modules); --i >= 0; ) {
			char *k = getdictkey(modules, i);
			if (k != NULL) {
				object *m = dictlookup(modules, k);
				if (m != NULL && is_moduleobject(m)) {
					object *d = getmoduledict(m);
					if (d != NULL && is_dictobject(d)) {
						cleardict(d);
					}
				}
			}
		}
		cleardict(modules);
	}
	DECREF(modules);
}

/* Initialize built-in modules when first imported */

extern struct {
	char *name;
	void (*initfunc)();
} inittab[];

static int
init_builtin(char *name)
{
	for (int i = 0; inittab[i].name != NULL; i++) {
		if (strcmp(name, inittab[i].name) == 0) {
			(*inittab[i].initfunc)();
			return 1;
		}
	}
	return 0;
}
