/* Parser generator main program */

/* This expects a filename containing the grammar as argv[1] (UNIX).
   It writes its output on two files in the current directory:
   	- "graminit.c" gets the grammar as a bunch of initialized data
   	- "graminit.h" gets the grammar's non-terminals as #defines.
   Error messages and status info during the generation process are
   written to stdout, or sometimes to stderr. */

#include "malloc.h"
#include "grammar.h"
#include "node.h"
#include "parsetok.h"
#include "pgen.h"

int debugging;

/* Forward */
grammar *getgrammar(char *filename);

int
main(int argc, char **argv)
{
	grammar *g;
	node *n;
	FILE *fp;
	char *filename;

	if (argc != 2) {
		fprintf(stderr, "usage: %s grammar\n", argv[0]);
		exit(2);
	}
	filename = argv[1];
	g = getgrammar(filename);
	fp = fopen("graminit.c", "w");
	if (fp == NULL) {
		perror("graminit.c");
		exit(1);
	}
	printf("Writing graminit.c ...\n");
	printgrammar(g, fp);
	fclose(fp);
	fp = fopen("graminit.h", "w");
	if (fp == NULL) {
		perror("graminit.h");
		exit(1);
	}
	printf("Writing graminit.h ...\n");
	printnonterminals(g, fp);
	fclose(fp);
	exit(0);
}

grammar *
getgrammar(char *filename)
{
	FILE *fp;
	node *n;
	grammar *g0, *g;
	
	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror(filename);
		exit(1);
	}
	g0 = meta_grammar();
	n = NULL;
	parsefile(fp, filename, g0, g0->g_start, (char *)NULL, (char *)NULL, &n);
	fclose(fp);
	if (n == NULL) {
		fprintf(stderr, "Parsing error.\n");
		exit(1);
	}
	g = pgen(n);
	if (g == NULL) {
		printf("Bad grammar.\n");
		exit(1);
	}
	return g;
}

/* XXX TO DO:
   - check for duplicate definitions of names (instead of fatal err)
*/
