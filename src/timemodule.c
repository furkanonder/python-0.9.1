/* Time module */

#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "object.h"
#include "intobject.h"
#include "methodobject.h"
#include "errors.h"
#include "modsupport.h"

/* Time methods */

static long
millitimer()
{
	struct timeval t;
	struct timezone tz;

	if (gettimeofday(&t, &tz) != 0) {
		return -1;
    }
	return t.tv_sec * 1000 + t.tv_usec / 1000;
}

static void
millisleep(long msecs)
{
	struct timeval t;
	t.tv_sec = msecs / 1000;
	t.tv_usec = (msecs % 1000) * 1000;

	(void)select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &t);
}

static object *
time_time(object *self, object *args)
{
	long secs;

	if (!getnoarg(args)) {
		return NULL;
    }
	secs = time((time_t *)NULL);
	return newintobject(secs);
}

static jmp_buf sleep_intr;

static void
sleep_catcher(int sig)
{
	longjmp(sleep_intr, 1);
}

static object *
time_sleep(object *self, object *args)
{
	int secs;

	void (*sigsave)(int);
	if (!getintarg(args, &secs)) {
		return NULL;
    }
	if (setjmp(sleep_intr)) {
		signal(SIGINT, sigsave);
		err_set(KeyboardInterrupt);
		return NULL;
	}
	sigsave = signal(SIGINT, SIG_IGN);
	if (sigsave != (void (*)(int)) SIG_IGN) {
		signal(SIGINT, sleep_catcher);
    }
	sleep(secs);
	signal(SIGINT, sigsave);
	INCREF(None);
	return None;
}

static object *
time_millisleep(object *self, object *args)
{
	long msecs;

	void (*sigsave)(int);
	if (!getlongarg(args, &msecs)) {
		return NULL;
    }
	if (setjmp(sleep_intr)) {
		signal(SIGINT, sigsave);
		err_set(KeyboardInterrupt);
		return NULL;
	}
	sigsave = signal(SIGINT, SIG_IGN);
	if (sigsave != (void (*)(int)) SIG_IGN) {
		signal(SIGINT, sleep_catcher);
    }
	millisleep(msecs);
	signal(SIGINT, sigsave);
	INCREF(None);
	return None;
}

static object *
time_millitimer(object *self, object *args)
{
	long msecs;
	extern long millitimer();

	if (!getnoarg(args)) {
		return NULL;
    }
	msecs = millitimer();
	return newintobject(msecs);
}

static struct methodlist time_methods[] = {
	{"millisleep",	(method)time_millisleep},
	{"millitimer",	(method)time_millitimer},
	{"sleep",		(method)time_sleep},
	{"time",		(method)time_time},
	{NULL,		    NULL}   /* sentinel */
};

void
inittime()
{
	initmodule("time", time_methods);
}
