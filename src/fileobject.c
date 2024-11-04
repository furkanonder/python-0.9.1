/* File object implementation */

/* XXX This should become a built-in module 'io'.  It should support more
   functionality, better exception handling for invalid calls, etc.
   (Especially reading on a write-only file or vice versa!)  It should also
   cooperate with posix to support popen(), which should share most code but
   have a special close function. */

#include <string.h>

#include "object.h"
#include "objimpl.h"
#include "intobject.h"
#include "stringobject.h"
#include "methodobject.h"
#include "fileobject.h"
#include "errors.h"
#include "malloc.h"
#include "errno.h"
#ifndef errno
extern int errno;
#endif

typedef struct {
	OB_HEAD
	FILE 	*f_fp;
	object	*f_name;
	object	*f_mode;
	/* XXX Should move the 'need space' on printing flag here */
} fileobject;

FILE *
getfilefile(object *f)
{
	if (!is_fileobject(f)) {
		err_badcall();
		return NULL;
	}
	return ((fileobject *)f)->f_fp;
}

object *
newopenfileobject(FILE *fp, char *name, char *mode)
{
	fileobject *f = NEWOBJ(fileobject, &Filetype);

	if (f == NULL) {
		return NULL;
    }
	f->f_fp = NULL;
	f->f_name = newstringobject(name);
	f->f_mode = newstringobject(mode);
	if (f->f_name == NULL || f->f_mode == NULL) {
		DECREF(f);
		return NULL;
	}
	f->f_fp = fp;
	return (object *)f;
}

object *
newfileobject(char *name, char *mode)
{
	fileobject *f = (fileobject *)newopenfileobject((FILE *)NULL, name, mode);
	FILE *fp;

	if (f == NULL) {
		return NULL;
    }
	f->f_fp = fopen(name, mode);
	if (f->f_fp == NULL) {
		err_errno(RuntimeError);
		DECREF(f);
		return NULL;
	}
	return (object *)f;
}

/* Methods */

static void
file_dealloc(fileobject *f)
{
	if (f->f_fp != NULL) {
		fclose(f->f_fp);
    }
	if (f->f_name != NULL) {
		DECREF(f->f_name);
    }
	if (f->f_mode != NULL) {
		DECREF(f->f_mode);
    }
	free((char *)f);
}

static void
file_print(fileobject *f, FILE *fp, int flags)
{
	fprintf(fp, "<%s file ", f->f_fp == NULL ? "closed" : "open");
	printobject(f->f_name, fp, flags);
	fprintf(fp, ", mode ");
	printobject(f->f_mode, fp, flags);
	fprintf(fp, ">");
}

static object *
file_repr(fileobject *f)
{
	char buf[300];

	/* XXX This differs from file_print if the filename contains quotes or
       other funny characters. */
	sprintf(buf, "<%s file '%.256s', mode '%.10s'>",
            f->f_fp == NULL ? "closed" : "open", getstringvalue(f->f_name),
			getstringvalue(f->f_mode));
	return newstringobject(buf);
}

static object *
file_close(fileobject *f, object *args)
{
	if (args != NULL) {
		err_badarg();
		return NULL;
	}
	if (f->f_fp != NULL) {
		fclose(f->f_fp);
		f->f_fp = NULL;
	}
	INCREF(None);
	return None;
}

static object *
file_read(fileobject *f, object *args)
{
	int n;
	object *v;

	if (f->f_fp == NULL) {
		err_badarg();
		return NULL;
	}
	if (args == NULL || !is_intobject(args)) {
		err_badarg();
		return NULL;
	}
	n = getintvalue(args);
	if (n < 0) {
		err_badarg();
		return NULL;
	}
	v = newsizedstringobject((char *)NULL, n);
	if (v == NULL) {
		return NULL;
    }
	n = fread(getstringvalue(v), 1, n, f->f_fp);
	/* EOF is reported as an empty string */
	/* XXX should detect real I/O errors? */
	resizestring(&v, n);
	return v;
}

/* XXX Should this be unified with raw_input()? */
static object *
file_readline(fileobject *f, object *args)
{
	int n;
	object *v;

	if (f->f_fp == NULL) {
		err_badarg();
		return NULL;
	}
	if (args == NULL) {
		n = 10000; /* XXX should really be unlimited */
	}
	else if (is_intobject(args)) {
		n = getintvalue(args);
		if (n < 0) {
			err_badarg();
			return NULL;
		}
	}
	else {
		err_badarg();
		return NULL;
	}
	v = newsizedstringobject((char *)NULL, n);
	if (v == NULL) {
		return NULL;
    }
	n = n + 1;
	if (fgets(getstringvalue(v), n, f->f_fp) == NULL) {
		/* EOF is reported as an empty string */
		/* XXX should detect real I/O errors? */
		n = 0;
	}
	else {
		n = strlen(getstringvalue(v));
	}
	resizestring(&v, n);
	return v;
}

static object *
file_write(fileobject *f, object *args)
{
	int n, n2;

	if (f->f_fp == NULL) {
		err_badarg();
		return NULL;
	}
	if (args == NULL || !is_stringobject(args)) {
		err_badarg();
		return NULL;
	}
	errno = 0;
	n2 = fwrite(getstringvalue(args), 1, n = getstringsize(args), f->f_fp);
	if (n2 != n) {
		if (errno == 0) {
			errno = EIO;
        }
		err_errno(RuntimeError);
		return NULL;
	}
	INCREF(None);
	return None;
}

static struct methodlist file_methods[] = {
	{"write",		(method)file_write},
	{"read",		(method)file_read},
	{"readline",	(method)file_readline},
	{"close",		(method)file_close},
	{NULL,			NULL}	/* sentinel */
};

static object *
file_getattr(fileobject *f, char *name)
{
	return findmethod(file_methods, (object *)f, name);
}

typeobject Filetype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"file",
	sizeof(fileobject),
	0,
	(destructor)file_dealloc,	/*tp_dealloc*/
	(printfunc)file_print,		/*tp_print*/
	(getattrfunc)file_getattr,	/*tp_getattr*/
	0,							/*tp_setattr*/
	0,							/*tp_compare*/
	(reprfunc)file_repr,		/*tp_repr*/
};
