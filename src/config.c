/* Configurable Python configuration file */

#include <stdio.h>

#define PYTHONPATH ".:/usr/local/lib/python"

extern char *getenv();

char *
getpythonpath()
{
	char *path = getenv("PYTHONPATH");

	if (path == 0) {
		path = PYTHONPATH;
    }
	return path;
}

/* Table of built-in modules. These are initialized when first imported. */

/* Standard modules */
extern void inittime();
extern void initmath();
extern void initregexp();
extern void initposix();

struct {
	char *name;
	void (*initfunc)();
} inittab[] = {
	/* Standard modules */
	{"time",	inittime},
	{"math",	initmath},
	{"regexp",	initregexp},
	{"posix",	initposix},
	{0,			0}	/* Sentinel */
};
