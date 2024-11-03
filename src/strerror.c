#include <stdio.h>

extern int sys_nerr;
extern char *sys_errlist[];

char *
strerror(int err)
{
	static char buf[20];

	if (err >= 0 && err < sys_nerr) {
		return sys_errlist[err];
    }
	sprintf(buf, "Unknown errno %d", err);
	return buf;
}
