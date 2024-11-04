#ifndef Py_PYTHONRUN_H
#define Py_PYTHONRUN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "node.h"

/* Interfaces to parse and execute pieces of python code */

void initall(void);
int run(FILE *, char *);
int run_script(FILE *, char *);
int run_tty_1(FILE *, char *);
int run_tty_loop(FILE *, char *);
int parse_string(char *, int, struct _node **);
int parse_file(FILE *, char *, int, struct _node **);
void print_error(void);
void goaway(int);

object *eval_node(struct _node *, char *, object *, object *);
object *run_string(char *, int, object *, object *);
object *run_file(FILE *, char *, int, object *, object *);
object *run_err_node(int, struct _node *, char *, object *, object *);
object *run_node(struct _node *, char *, object *, object *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_PYTHONRUN_H */
