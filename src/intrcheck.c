/* Check for interrupts */

#ifndef OK

/* Default version -- for real operating systems and for Standard C */

#include <stdio.h>
#include <signal.h>
#include "sigtype.h"

static int interrupted;

static SIGTYPE
intcatcher(int sig)
{
	interrupted = 1;

	signal(SIGINT, intcatcher);
}

void
initintr()
{
	if (signal(SIGINT, SIG_IGN) != SIG_IGN) {
		signal(SIGINT, intcatcher);
    }
}

int
intrcheck()
{
	if (!interrupted) {
		return 0;
    }
	interrupted = 0;
	return 1;
}

#endif /* !OK */
