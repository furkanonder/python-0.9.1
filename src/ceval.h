/* Interface to execute compiled code */
/* This header depends on "compile.h" */

object *eval_code(codeobject *, object *, object *, object *);
object *getglobals(void);
object *getlocals(void);

void printtraceback(FILE *);
void flushline();