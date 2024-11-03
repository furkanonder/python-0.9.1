/* Traceback interface */

int tb_here(struct _frame *, int, int);
object *tb_fetch(void);
int tb_store(object *);
int tb_print(object *, FILE *);
