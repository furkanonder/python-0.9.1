/* Module object interface */

extern typeobject Moduletype;

#define is_moduleobject(op) ((op)->ob_type == &Moduletype)

extern object *newmoduleobject(char *);
extern object *getmoduledict(object *);
extern char *getmodulename(object *);
