/* Parse tree node implementation */

#include "malloc.h"
#include "node.h"

#define XXX 3 /* Node alignment factor to speed up realloc */
#define XXXROUNDUP(n) ((n) == 1 ? 1 : ((n) + XXX - 1) / XXX * XXX)

node *
newtree(int type)
{
	node *n = NEW(node, 1);

	if (n == NULL) {
		return NULL;
    }
	n->n_type = type;
	n->n_str = NULL;
	n->n_lineno = 0;
	n->n_nchildren = 0;
	n->n_child = NULL;
	return n;
}

node *
addchild(register node *n1, int type, char *str, int lineno)
{
	register int nch = n1->n_nchildren, nch1 = nch + 1;
	register node *n;

	if (XXXROUNDUP(nch) < nch1) {
		n = n1->n_child;
		nch1 = XXXROUNDUP(nch1);
		RESIZE(n, node, nch1);
		if (n == NULL) {
			return NULL;
        }
		n1->n_child = n;
	}
	n = &n1->n_child[n1->n_nchildren++];
	n->n_type = type;
	n->n_str = str;
	n->n_lineno = lineno;
	n->n_nchildren = 0;
	n->n_child = NULL;
	return n;
}

static void
freechildren(node *n)
{
	for (int i = NCH(n); --i >= 0;) {
		freechildren(CHILD(n, i));
	}
	if (n->n_child != NULL) {
		DEL(n->n_child);
	}
	if (STR(n) != NULL) {
		DEL(STR(n));
	}
}

void
freetree(node *n)
{
	if (n != NULL) {
		freechildren(n);
		DEL(n);
	}
}
