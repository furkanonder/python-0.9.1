/* Class object implementation */

#include "allobjects.h"
#include "structmember.h"

typedef struct {
	OB_HEAD
	object	*cl_bases;		/* A tuple */
	object	*cl_methods;	/* A dictionary */
} classobject;

/* object *bases: NULL or tuple of classobjects! */
object *
newclassobject(object *bases, object *methods)
{
	classobject *op;
	op = NEWOBJ(classobject, &Classtype);

	if (op == NULL) {
		return NULL;
    }
	if (bases != NULL) {
		INCREF(bases);
    }
	op->cl_bases = bases;
	INCREF(methods);
	op->cl_methods = methods;
	return (object *)op;
}

/* Class methods */

static void
class_dealloc(classobject *op)
{
	if (op->cl_bases != NULL) {
		DECREF(op->cl_bases);
    }
	DECREF(op->cl_methods);
	free((ANY *)op);
}

static object *
class_getattr(register classobject *op, register char *name)
{
	register object *v;
	v = dictlookup(op->cl_methods, name);

	if (v != NULL) {
		INCREF(v);
		return v;
	}
	if (op->cl_bases != NULL) {
		for (int i = 0; i < gettuplesize(op->cl_bases); i++) {
			v = class_getattr((classobject *)gettupleitem(op->cl_bases, i),
                               name);
			if (v != NULL) {
				return v;
            }
			err_clear();
		}
	}
	err_setstr(NameError, name);
	return NULL;
}

typeobject Classtype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"class",
	sizeof(classobject),
	0,
	(destructor)class_dealloc,	/*tp_dealloc*/
	0,							/*tp_print*/
	(getattrfunc)class_getattr,	/*tp_getattr*/
	0,							/*tp_setattr*/
	0,							/*tp_compare*/
	0,							/*tp_repr*/
	0,							/*tp_as_number*/
	0,							/*tp_as_sequence*/
	0,							/*tp_as_mapping*/
};

/* We're not done yet: next, we define class member objects... */

typedef struct {
	OB_HEAD
	classobject	*cm_class;	/* The class object */
	object		*cm_attr;	/* A dictionary */
} classmemberobject;

object *
newclassmemberobject(register object *class)
{
	register classmemberobject *cm;

	if (!is_classobject(class)) {
		err_badcall();
		return NULL;
	}
	cm = NEWOBJ(classmemberobject, &Classmembertype);
	if (cm == NULL) {
		return NULL;
    }
	INCREF(class);
	cm->cm_class = (classobject *)class;
	cm->cm_attr = newdictobject();
	if (cm->cm_attr == NULL) {
		DECREF(cm);
		return NULL;
	}
	return (object *)cm;
}

/* Class member methods */

static void
classmember_dealloc(register classmemberobject *cm)
{
	DECREF(cm->cm_class);
	if (cm->cm_attr != NULL) {
		DECREF(cm->cm_attr);
    }
	free((ANY *)cm);
}

static object *
classmember_getattr(register classmemberobject *cm, register char *name)
{
	register object *v = dictlookup(cm->cm_attr, name);

	if (v != NULL) {
		INCREF(v);
		return v;
	}
	v = class_getattr(cm->cm_class, name);
	if (v == NULL) {
		return v; /* class_getattr() has set the error */
    }
	if (is_funcobject(v)) {
		object *w = newclassmethodobject(v, (object *)cm);
		DECREF(v);
		return w;
	}
	DECREF(v);
	err_setstr(NameError, name);
	return NULL;
}

static int
classmember_setattr(classmemberobject *cm, char *name, object *v)
{
	if (v == NULL) {
		return dictremove(cm->cm_attr, name);
    }
	else {
		return dictinsert(cm->cm_attr, name, v);
    }
}

typeobject Classmembertype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"class member",
	sizeof(classmemberobject),
	0,
	(destructor)classmember_dealloc,	/*tp_dealloc*/
	0,									/*tp_print*/
	(getattrfunc)classmember_getattr,	/*tp_getattr*/
	(setattrfunc)classmember_setattr,	/*tp_setattr*/
	0,									/*tp_compare*/
	0,									/*tp_repr*/
	0,									/*tp_as_number*/
	0,									/*tp_as_sequence*/
	0,									/*tp_as_mapping*/
};

/* And finally, here are class method objects */
/* (Really methods of class members) */

typedef struct {
	OB_HEAD
	object	*cm_func;	/* The method function */
	object	*cm_self;	/* The object to which this applies */
} classmethodobject;

object *
newclassmethodobject(object *func, object *self)
{
	register classmethodobject *cm;

	if (!is_funcobject(func)) {
		err_badcall();
		return NULL;
	}
	cm = NEWOBJ(classmethodobject, &Classmethodtype);
	if (cm == NULL) {
		return NULL;
    }
	INCREF(func);
	cm->cm_func = func;
	INCREF(self);
	cm->cm_self = self;
	return (object *)cm;
}

object *
classmethodgetfunc(register object *cm)
{
	if (!is_classmethodobject(cm)) {
		err_badcall();
		return NULL;
	}
	return ((classmethodobject *)cm)->cm_func;
}

object *
classmethodgetself(register object *cm)
{
	if (!is_classmethodobject(cm)) {
		err_badcall();
		return NULL;
	}
	return ((classmethodobject *)cm)->cm_self;
}

/* Class method methods */

#define OFF(x) offsetof(classmethodobject, x)

static struct memberlist classmethod_memberlist[] = {
	{"cm_func",	T_OBJECT,	OFF(cm_func)},
	{"cm_self",	T_OBJECT,	OFF(cm_self)},
	{NULL}	/* Sentinel */
};

static object *
classmethod_getattr(register classmethodobject *cm, char *name)
{
	return getmember((char *)cm, classmethod_memberlist, name);
}

static void
classmethod_dealloc(register classmethodobject *cm)
{
	DECREF(cm->cm_func);
	DECREF(cm->cm_self);
	free((ANY *)cm);
}

typeobject Classmethodtype = {
	OB_HEAD_INIT(&Typetype)
	0,
	"class method",
	sizeof(classmethodobject),
	0,
	(destructor)classmethod_dealloc,	/*tp_dealloc*/
	0,									/*tp_print*/
	(getattrfunc)classmethod_getattr,	/*tp_getattr*/
	0,									/*tp_setattr*/
	0,									/*tp_compare*/
	0,									/*tp_repr*/
	0,									/*tp_as_number*/
	0,									/*tp_as_sequence*/
	0,									/*tp_as_mapping*/
};
