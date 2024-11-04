#ifndef Py_FLOATOBJECT_H
#define Py_FLOATOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Float object interface */

/* floatobject represents a (double precision) floating point number. */
typedef struct {
	OB_HEAD
	double ob_fval;
} floatobject;

extern typeobject Floattype;
#define is_floatobject(op) ((op)->ob_type == &Floattype)

extern object *newfloatobject(double);
extern double getfloatvalue(object *);

/* Macro, trading safety for speed */
#define GETFLOATVALUE(op) ((op)->ob_fval)

#ifdef __cplusplus
}
#endif

#endif /* !Py_FLOATOBJECT_H */
