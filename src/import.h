/* Module definition and import interface */

object *get_modules(void);
object *add_module(char *name);
object *import_module(char *name);
object *reload_module(object *m);
void doneimport(void);
extern void initimport();