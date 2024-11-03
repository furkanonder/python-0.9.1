#ifndef Py_CEVAL_H
#define Py_CEVAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Interface to execute compiled code */
/* This header depends on "compile.h" */

object *eval_code(codeobject *, object *, object *, object *);
object *getglobals(void);
object *getlocals(void);

void printtraceback(FILE *);
void flushline();

#ifdef __cplusplus
}
#endif

#endif /* !Py_CEVAL_H */
