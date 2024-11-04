/* List a node on a file */

#include "token.h"
#include "node.h"

static int level, atbol;

static void
list1node(FILE *fp, node *n)
{
	if (n == 0) {
		return;
	}
	if (ISNONTERMINAL(TYPE(n))) {
		for (int i = 0; i < NCH(n); i++) {
			list1node(fp, CHILD(n, i));
		}
	}
	else if (ISTERMINAL(TYPE(n))) {
		switch (TYPE(n)) {
			case INDENT:
				++level;
				break;

			case DEDENT:
				--level;
				break;

			default:
				if (atbol) {
					for (int i = 0; i < level; ++i) {
						fprintf(fp, "\t");
					}
					atbol = 0;
				}

			if (TYPE(n) == NEWLINE) {
				if (STR(n) != NULL) {
					fprintf(fp, "%s", STR(n));
				}
				fprintf(fp, "\n");
				atbol = 1;
			}
			else {
				fprintf(fp, "%s ", STR(n));
			}
			break;
		}
	}
	else {
		fprintf(fp, "? ");
	}
}

void
listnode(FILE *fp, node *n)
{
	level = 0;
	atbol = 1;
	list1node(fp, n);
}

void
listtree(node *n)
{
	listnode(stdout, n);
}
