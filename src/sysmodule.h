#ifndef Py_SYSMODULE_H
#define Py_SYSMODULE_H

#ifdef __cplusplus
extern "C" {
#endif

/* System module interface */

object *sysget(char *);
int sysset(char *, object *);
FILE *sysgetfile(char *, FILE *);
void initsys(void);
extern void setpythonpath(char *path);
extern void setpythonargv(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif /* !Py_SYSMODULE_H */
