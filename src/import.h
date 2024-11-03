#ifndef Py_IMPORT_H
#define Py_IMPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Module definition and import interface */

object *get_modules(void);
object *add_module(char *name);
object *import_module(char *name);
object *reload_module(object *m);
void doneimport(void);
extern void initimport();

#ifdef __cplusplus
}
#endif

#endif /* !Py_IMPORT_H */
