#ifndef Py_CONFIG_H
#define Py_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

extern void initargs(int *p_argc, char ***p_argv);
extern void initcalls();
extern void donecalls();

#ifdef __cplusplus
}
#endif

#endif /* !Py_CONFIG_H */
