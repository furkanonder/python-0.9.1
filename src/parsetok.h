#ifndef Py_PARSETOK_H
#define Py_PARSETOK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Parser-tokenizer link interface */

extern int parsestring(char *, grammar *, int start, node **n_ret);
extern int parsefile(FILE *, char *, grammar *, int start, char *ps1,
                     char *ps2, node **n_ret);

#ifdef __cplusplus
}
#endif

#endif /* !Py_PARSETOK_H */