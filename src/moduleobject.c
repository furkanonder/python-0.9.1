/* Module object implementation */

#include <string.h>

#include "object.h"
#include "objimpl.h"
#include "stringobject.h"
#include "dictobject.h"
#include "moduleobject.h"
#include "errors.h"
#include "malloc.h"

typedef struct {
	OB_HEAD
	object *md_name;
	object *md_dict;
} moduleobject;

object *
newmoduleobject(char *name)
{
	moduleobject *m = NEWOBJ(moduleobject, &Moduletype);

	if (m == NULL) {
		return NULL;
    }
	m->md_name = newstringobject(name);
	m->md_dict = newdictobject();
	if (m->md_name == NULL || m->md_dict == NULL) {
		DECREF(m);
		return NULL;
	}
	return (object *)m;
}

object *
getmoduledict(object *m)
{
	if (!is_moduleobject(m)) {
		err_badcall();
		return NULL;
	}
	return ((moduleobject *)m)->md_dict;
}

char *
getmodulename(object *m)
{
	if (!is_moduleobject(m)) {
		err_badarg();
		return NULL;
	}
	return getstringvalue(((moduleobject *)m)->md_name);
}

/* Methods */

static void
module_dealloc(moduleobject *m)
{
	if (m->md_name != NULL) {
		DECREF(m->md_name);
    }
	if (m->md_dict != NULL) {
		DECREF(m->md_dict);
    }
	free((char *)m);
}

static void
module_print(moduleobject *m, FILE *fp, int flags)
{
	fprintf(fp, "<module '%s'>", getstringvalue(m->md_name));
}

static object *
module_repr(moduleobject *m)
{
	char buf[100];

	sprintf(buf, "<module '%.80s'>", getstringvalue(m->md_name));
	return newstringobject(buf);
}

static object *
module_getattr(moduleobject *m, char *name)
{
	object *res;

	if (strcmp(name, "__dict__") == 0) {
		INCREF(m->md_dict);
		return m->md_dict;
	}
	if (strcmp(name, "__name__") == 0) {
		INCREF(m->md_name);
		return m->md_name;
	}
	res = dictlookup(m->md_dict, name);
	if (res == NULL) {
		err_setstr(NameError, name);
    }
	else {
		INCREF(res);
    }
	return res;
}

static int
module_setattr(moduleobject *m, char *name, object *v)
{
	if (strcmp(name, "__dict__") == 0 || strcmp(name, "__name__") == 0) {
		err_setstr(NameError, "can't assign to reserved member name");
		return -1;
	}
	if (v == NULL) {
		return dictremove(m->md_dict, name);
    }
	else {
		return dictinsert(m->md_dict, name, v);
    }
}

typeobject Moduletype = {
	OB_HEAD_INIT(&Typetype)
	0,									/*ob_size*/
	"module",							/*tp_name*/
	sizeof(moduleobject),				/*tp_size*/
	0,									/*tp_itemsize*/
	(destructor)module_dealloc,			/*tp_dealloc*/
	(printfunc)module_print,			/*tp_print*/
	(getattrfunc)module_getattr,		/*tp_getattr*/
	(setattrfunc)module_setattr,		/*tp_setattr*/
	0,									/*tp_compare*/
	(reprfunc)module_repr,				/*tp_repr*/
};
