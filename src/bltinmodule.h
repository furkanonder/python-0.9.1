#ifndef Py_BLTINMODULE_H
#define Py_BLTINMODULE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Built-in module interface */

extern object *getbuiltin(char *);
extern void initbuiltin();

#ifdef __cplusplus
}
#endif

#endif /* !Py_BLTINMODULE_H */
