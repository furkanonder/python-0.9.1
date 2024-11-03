/* Interruptable version of fgets().
   Return < 0 for interrupted, 1 for EOF, 0 for valid input. */

/* XXX This uses longjmp() from a signal out of fgets().
   Should use read() instead?! */

#include "pgenheaders.h"

#include <signal.h>
#include <setjmp.h>

#include "errcode.h"
#include "sigtype.h"
#include "fgetsintr.h"
#include "intrcheck.h"

#define sig_block()		/*empty*/
#define sig_unblock()	/*empty*/

static jmp_buf jback;
static void catcher(int);

static void
catcher(int sig)
{
	longjmp(jback, 1);
}

int
fgets_intr(char *buf, int size, FILE *fp)
{
	int ret;
	SIGTYPE (*sigsave)();
	
	if (setjmp(jback)) {
		clearerr(fp);
		signal(SIGINT, sigsave);
		return E_INTR;
	}
	
	sigsave = signal(SIGINT, SIG_IGN);
	if (sigsave != SIG_IGN) {
		signal(SIGINT, catcher);
	}
	if (intrcheck()) {
		ret = E_INTR;
    }
	else {
		sig_block();
		ret = (fgets(buf, size, fp) == NULL) ? E_EOF : E_OK;
		sig_unblock();
	}
	
	if (sigsave != SIG_IGN) {
		signal(SIGINT, sigsave);
    }
	return ret;
}
