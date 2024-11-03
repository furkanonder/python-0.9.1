#ifndef Py_TRACEBACK_H
#define Py_TRACEBACK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Traceback interface */

int tb_here(struct _frame *, int, int);
object *tb_fetch(void);
int tb_store(object *);
int tb_print(object *, FILE *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_TRACEBACK_H */
