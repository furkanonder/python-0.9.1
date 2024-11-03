/* Method object interface */

extern typeobject Methodtype;

#define is_methodobject(op) ((op)->ob_type == &Methodtype)

typedef object *(*method)(object *, object *);

extern object *newmethodobject(char *, method, object *);
extern method getmethod(object *);
extern object *getself(object *);

struct methodlist {
	char    *ml_name;
	method  ml_meth;
};

extern object *findmethod(struct methodlist *, object *, char *);
