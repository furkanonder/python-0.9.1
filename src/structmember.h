#ifndef Py_STRUCTMEMBER_H
#define Py_STRUCTMEMBER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Interface to map C struct members to Python object attributes */

/* An array of memberlist structures defines the name, type and offset of
   selected members of a C structure.  These can be read by getmember() and set
   by setmember() (except if their READONLY flag is set).  The array must be
   terminated with an entry whose name pointer is NULL. */
struct memberlist {
	char *name;
	int type;
	int offset;
	int readonly;
};

/* Types */
#define T_SHORT		0
#define T_INT		1
#define T_LONG		2
#define T_FLOAT		3
#define T_DOUBLE	4
#define T_STRING	5
#define T_OBJECT	6

/* Readonly flag */
#define READONLY	1
#define RO			READONLY	/* Shorthand */

object *getmember(char *, struct memberlist *, char *);
int setmember(char *, struct memberlist *, char *, object *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_STRUCTMEMBER_H */
