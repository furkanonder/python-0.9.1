/* POSIX module implementation */

#include <signal.h>
#include <string.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#ifdef SYSV
#include <dirent.h>
#define direct dirent
#else
#include <sys/dir.h>
#endif
#include <unistd.h>

#include "allobjects.h"
#include "modsupport.h"

extern char *strerror(int);

/* Return a dictionary corresponding to the POSIX environment table */
extern char **environ;

static object *
convertenviron()
{
	object *d = newdictobject();
	char **e;

	if (d == NULL) {
		return NULL;
    }
	if (environ == NULL) {
		return d;
    }
	/* XXX This part ignores errors */
	for (e = environ; *e != NULL; e++) {
		object *v;
		char *p = strchr(*e, '=');
		if (p == NULL) {
			continue;
        }
		v = newstringobject(p + 1);
		if (v == NULL) {
			continue;
        }
		*p = '\0';
		(void)dictinsert(d, *e, v);
		*p = '=';
		DECREF(v);
	}
	return d;
}

static object *PosixError; /* Exception posix.error */

/* Set a POSIX-specific error from errno, and return NULL */
static object *
posix_error()
{
	return err_errno(PosixError);
}

/* POSIX generic methods */

static object *
posix_1str(object *args, int (*func)(const char *))
{
	object *path1;

	if (!getstrarg(args, &path1)) {
		return NULL;
    }
	if ((*func)(getstringvalue(path1)) < 0) {
		return posix_error();
    }
	INCREF(None);
	return None;
}

static object *
posix_2str(object *args, int (*func)(const char *, const char *))
{
	object *path1, *path2;

	if (!getstrstrarg(args, &path1, &path2)) {
		return NULL;
    }
	if ((*func)(getstringvalue(path1), getstringvalue(path2)) < 0) {
		return posix_error();
    }
	INCREF(None);
	return None;
}

static object *
posix_strint(object *args, int (*func)(const char *, int))
{
	object *path1;
	int i;

	if (!getstrintarg(args, &path1, &i)) {
		return NULL;
    }
	if ((*func)(getstringvalue(path1), i) < 0) {
		return posix_error();
    }
	INCREF(None);
	return None;
}

static object *
posix_do_stat(object *self, object *args,
              int (*statfunc)(const char *, struct stat *))
{
	struct stat st;
	object *path, *v;

	if (!getstrarg(args, &path)) {
		return NULL;
    }
	if ((*statfunc)(getstringvalue(path), &st) != 0) {
		return posix_error();
    }
	v = newtupleobject(10);
	if (v == NULL) {
		return NULL;
    }
#define SET(i, st_member) settupleitem(v, i, newintobject((long)st.st_member))
	SET(0, st_mode);
	SET(1, st_ino);
	SET(2, st_dev);
	SET(3, st_nlink);
	SET(4, st_uid);
	SET(5, st_gid);
	SET(6, st_size);
	SET(7, st_atime);
	SET(8, st_mtime);
	SET(9, st_ctime);
#undef SET
	if (err_occurred()) {
		DECREF(v);
		return NULL;
	}
	return v;
}

/* POSIX methods */

static object *
posix_chdir(object *self, object *args)
{
	extern int chdir(const char *);
	return posix_1str(args, chdir);
}

static int
chmod_wrapper(const char *path, int mode)
{
	return chmod(path, (mode_t)mode);
}

static object *
posix_chmod(object *self, object *args)
{
	extern int chmod(const char *, mode_t);
	return posix_strint(args, chmod_wrapper);
}

static object *
posix_getcwd(object *self, object *args)
{
	char buf[1026];

	if (!getnoarg(args)) {
		return NULL;
    }
	if (getcwd(buf, sizeof buf) == NULL) {
		return posix_error();
    }
	return newstringobject(buf);
}

static object *
posix_link(object *self, object *args)
{
	extern int link(const char *, const char *);
	return posix_2str(args, link);
}

static object *
posix_listdir(object *self, object *args)
{
	object *name, *d, *v;
	DIR *dirp;
	struct direct *ep;

	if (!getstrarg(args, &name)) {
		return NULL;
    }
	if ((dirp = opendir(getstringvalue(name))) == NULL) {
		return posix_error();
    }
	if ((d = newlistobject(0)) == NULL) {
		closedir(dirp);
		return NULL;
	}
	while ((ep = readdir(dirp)) != NULL) {
		v = newstringobject(ep->d_name);
		if (v == NULL) {
			DECREF(d);
			d = NULL;
			break;
		}
		if (addlistitem(d, v) != 0) {
			DECREF(v);
			DECREF(d);
			d = NULL;
			break;
		}
		DECREF(v);
	}
	closedir(dirp);
	return d;
}

static int
mkdir_wrapper(const char *path, int mode)
{
	return mkdir(path, (mode_t)mode);
}

static object *
posix_mkdir(object *self, object *args)
{
	return posix_strint(args, mkdir_wrapper);
}

static object *
posix_rename(object *self, object *args)
{
	extern int rename(const char *, const char *);
	return posix_2str(args, rename);
}

static object *
posix_rmdir(object *self, object *args)
{
	extern int rmdir(const char *);
	return posix_1str(args, rmdir);
}

static object *
posix_stat(object *self, object *args)
{
	extern int stat(const char *, struct stat *);
	return posix_do_stat(self, args, stat);
}

static object *
posix_system(object *self, object *args)
{
	object *command;
	int sts;

	if (!getstrarg(args, &command)) {
		return NULL;
    }
	sts = system(getstringvalue(command));
	return newintobject((long)sts);
}

static object *
posix_umask(object *self, object *args)
{
	int i;

	if (!getintarg(args, &i)) {
		return NULL;
    }
	i = umask(i);
	if (i < 0) {
		return posix_error();
    }
	return newintobject((long)i);
}

static object *
posix_unlink(object *self, object *args)
{
	extern int unlink(const char *);
	return posix_1str(args, unlink);
}

static object *
posix_utimes(object *self, object *args)
{
	object *path;
	struct timeval tv[2];

	if (args == NULL || !is_tupleobject(args) || gettuplesize(args) != 2) {
		err_badarg();
		return NULL;
	}
	if (!getstrarg(gettupleitem(args, 0), &path)
        || !getlonglongargs(gettupleitem(args, 1), &tv[0].tv_sec,
        &tv[1].tv_sec))
    {
		return NULL;
    }
	tv[0].tv_usec = tv[1].tv_usec = 0;
	if (utimes(getstringvalue(path), tv) < 0) {
		return posix_error();
    }
	INCREF(None);
	return None;
}

#ifndef NO_LSTAT

static object *
posix_lstat(object *self, object *args)
{
	extern int lstat(const char *, struct stat *);
	return posix_do_stat(self, args, lstat);
}

static object *
posix_readlink(object *self, object *args)
{
	char buf[1024]; /* XXX Should use MAXPATHLEN */
	object *path;
	int n;
    
	if (!getstrarg(args, &path)) {
		return NULL;
    }
	n = readlink(getstringvalue(path), buf, sizeof buf);
	if (n < 0) {
		return posix_error();
    }
	return newsizedstringobject(buf, n);
}

static object *
posix_symlink(object *self, object *args)
{
	extern int symlink(const char *, const char *);
	return posix_2str(args, symlink);
}

#endif /* NO_LSTAT */

static struct methodlist posix_methods[] = {
	{"chdir",		(method)posix_chdir},
	{"chmod",		(method)posix_chmod},
	{"getcwd",		(method)posix_getcwd},
	{"link",		(method)posix_link},
	{"listdir",		(method)posix_listdir},
	{"mkdir",		(method)posix_mkdir},
	{"rename",		(method)posix_rename},
	{"rmdir",		(method)posix_rmdir},
	{"stat",		(method)posix_stat},
	{"system",		(method)posix_system},
	{"umask",		(method)posix_umask},
	{"unlink",		(method)posix_unlink},
	{"utimes",		(method)posix_utimes},
#ifndef NO_LSTAT
	{"lstat",		(method)posix_lstat},
	{"readlink",	(method)posix_readlink},
	{"symlink",		(method)posix_symlink},
#endif
	{NULL,			NULL}		 /* Sentinel */
};

void
initposix()
{
	object *m = initmodule("posix", posix_methods), *d = getmoduledict(m), *v;
	/* Initialize posix.environ dictionary */
	v = convertenviron();

	if (v == NULL || dictinsert(d, "environ", v) != 0) {
		fatal("can't define posix.environ");
    }
	DECREF(v);
	
	/* Initialize posix.error exception */
	PosixError = newstringobject("posix.error");
	if (PosixError == NULL || dictinsert(d, "error", PosixError) != 0) {
		fatal("can't define posix.error");
    }
}
