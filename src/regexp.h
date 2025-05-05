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

extern regexp *regcomp(char *exp);
extern int regexec(regexp *prog, char *string);
#ifdef MULTILINE
extern int reglexec(regexp *prog, char *string, int offset);
#endif
extern void regsub(regexp *prog, char *source, char *dest);
extern void regerror(char *msg);

#ifdef __cplusplus
}
#endif

#endif /* !Py_REGEXP_H */
