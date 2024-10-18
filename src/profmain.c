#include <stdio.h>
#include "string.h"

#include "PROTO.h"
#include "grammar.h"
#include "node.h"
#include "parsetok.h"
#include "graminit.h"
#include "tokenizer.h"
#include "errcode.h"
#include "malloc.h"

extern int _profile;

extern grammar gram; /* From graminit.c */

int debugging = 1;

FILE *getopenfile()
{
	char buf[256];
	char *p;
	FILE *fp;
	for (;;) {
		fprintf(stderr, "File name: ");
		if (fgets(buf, sizeof buf, stdin) == NULL) {
			fprintf(stderr, "EOF\n");
			exit(1);
		}
		p = strchr(buf, '\n');
		if (p != NULL)
			*p = '\0';
		if ((fp = fopen(buf, "r")) != NULL)
			break;
		fprintf(stderr, "Sorry, try again.\n");
	}
	return fp;
}

main()
{
	FILE *fp;
	_profile = 0;
	fp = getopenfile();
#if 0
	askgo("Start tokenizing");
	runtokenizer(fp);
	fseek(fp, 0L, 0);
#endif
	if (!gram.g_accel)
		addaccelerators(&gram);
	askgo("Start parsing");
	_profile = 1;
	runparser(fp);
	_profile = 0;
	freopen("prof.out", "w", stdout);
	DumpProfile();
	fflush(stdout);
	exit(0);
}

static int
runtokenizer(fp)
	FILE *fp;
{
	struct tok_state *tok;
	tok = tok_setupf(fp, "Tokenizing", ".");
	for (;;) {
		char *a, *b;
		register char *str;
		register int len;
		(void) tok_get(tok, &a, &b);
		if (tok->done != E_OK)
			break;
		len = b - a;
		str = NEW(char, len + 1);
		if (str == NULL) {
			fprintf(stderr, "no mem for next token");
			break;
		}
		strncpy(str, a, len);
		str[len] = '\0';
	}
	fprintf(stderr, "done (%d)\n", tok->done);
}

static
runparser(fp, start)
	FILE *fp;
{
	node *n;
	int ret;
	ret = parsefile(fp, &gram, module_input, "Parsing", ".", &n);
	fprintf(stderr, "done (%d)\n", ret);
#if 0
	_profile = 0;
	if (ret == E_DONE)
		listtree(n);
#endif
}

static int
askgo(prompt)
	char *prompt;
{
	char buf[256];
	fprintf(stderr, "%s: hit return when ready: ", prompt);
	fgets(buf, sizeof buf, stdin);
}
