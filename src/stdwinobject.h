/* Stdwin object interface */

extern typeobject Stdwintype;

#define is_stdwinobject(op) ((op)->ob_type == &Stdwintype)

extern object *newstdwinobject PROTO((void));
