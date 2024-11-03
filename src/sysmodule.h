/* System module interface */

object *sysget(char *);
int sysset(char *, object *);
FILE *sysgetfile(char *, FILE *);
void initsys(void);
extern void setpythonpath(char *path);
extern void setpythonargv(int argc, char **argv);