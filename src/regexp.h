#ifndef Py_REGEXP_H
#define Py_REGEXP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions etc. for regexp(3) routines.
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one. */

#define MULTILINE
#define NSUBEXP  10

typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int	regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

extern regexp *regcomp();
extern int regexec();
#ifdef MULTILINE
extern int reglexec();
#endif
extern void regsub();
extern void regerror(char *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_REGEXP_H */
