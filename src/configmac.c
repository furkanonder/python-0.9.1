/* Configuration using STDWIN on the Mac (THINK_C or MPW) */

#ifdef THINK_C
#define USE_STDWIN
#endif

#ifdef USE_STDWIN
#include "stdwin.h"
#endif

void
initargs(int *p_argc, char ***p_argv)
{
#ifdef USE_STDWIN

#ifdef THINK_C_3_0
	wsetstdio(1);
#else
	/* This printf() statement is really needed only
	   to initialize THINK C 4.0's stdio: */
	printf(
"Python 4.0, Copyright 1990 Stichting Mathematisch Centrum, Amsterdam\n");
#endif

	wargs(p_argc, p_argv);
#endif
}

void
initcalls()
{
}

void
donecalls()
{
#ifdef USE_STDWIN
	wdone();
#endif
}

#ifndef PYTHONPATH
/* On the Mac, the search path is a space-separated list of directories */
#define PYTHONPATH ": :lib :lib:stdwin :lib:mac :lib:demo"
#endif

char *
getpythonpath()
{
	return PYTHONPATH;
}


/* Table of built-in modules.
   These are initialized when first imported. */

/* Standard modules */
extern void inittime();
extern void initmath();
extern void initregexp();
	
/* Mac-specific modules */
extern void initmac();
#ifdef USE_STDWIN
extern void initstdwin();
#endif

struct {
	char *name;
	void (*initfunc)();
} inittab[] = {
	/* Standard modules */
	{"time",	inittime},
	{"math",	initmath},
	{"regexp",	initregexp},
	/* Mac-specific modules */
	{"mac",		initmac},
#ifdef USE_STDWIN
	{"stdwin",	initstdwin},
#endif
	{0,		0}		/* Sentinel */
};
