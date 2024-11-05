#ifndef Py_PGEN_H
#define Py_PGEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "node.h"

/* Parser generator interface */

extern grammar gram;
extern grammar *meta_grammar(void);
extern grammar *pgen(struct _node *);

#ifdef __cplusplus
}
#endif

#endif /* !Py_PGEN_H */
