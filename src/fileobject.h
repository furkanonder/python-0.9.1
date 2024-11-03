/* File object interface */

extern typeobject Filetype;

#define is_fileobject(op) ((op)->ob_type == &Filetype)

extern object *newfileobject(char *, char *);
extern object *newopenfileobject(FILE *, char *, char *);
extern FILE *getfilefile(object *);
