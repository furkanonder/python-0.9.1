/* Parser accelerator module */

/* The parser as originally conceived had disappointing performance. This
   module does some precomputation that speeds up the selection of a DFA based
   upon a token, turning a search through an array into a simple indexing
   operation. The parser now cannot work without the accelerators installed.
   Note that the accelerators are installed dynamically when the parser is
   initialized, they are not part of the static data structure written on
   graminit.[ch] by the parser generator. */

#include "malloc.h"
#include "grammar.h"
#include "token.h"

static void
fixstate(grammar *g, dfa *d, state *s)
{
	arc *a;
	int k, *accel, nl = g->g_ll.ll_nlabels;
	s->s_accept = 0;
	accel = NEW(int, nl);

	for (k = 0; k < nl; k++) {
		accel[k] = -1;
	}
	a = s->s_arc;
	for (k = s->s_narcs; --k >= 0; a++) {
		int lbl = a->a_lbl;
		label *l = &g->g_ll.ll_label[lbl];
		int type = l->lb_type;
		if (a->a_arrow >= (1 << 7)) {
			printf("XXX too many states!\n");
			continue;
		}
		if (ISNONTERMINAL(type)) {
			dfa *d1 = finddfa(g, type);
			int ibit;
			if (type - NT_OFFSET >= (1 << 7)) {
				printf("XXX too high nonterminal number!\n");
				continue;
			}
			for (ibit = 0; ibit < g->g_ll.ll_nlabels; ibit++) {
				if (testbit(d1->d_first, ibit)) {
					if (accel[ibit] != -1) {
						printf("XXX ambiguity!\n");
					}
					accel[ibit] = a->a_arrow | (1 << 7)
								  | ((type - NT_OFFSET) << 8);
				}
			}
		}
		else if (lbl == EMPTY) {
			s->s_accept = 1;
		}
		else if (lbl >= 0 && lbl < nl) {
			accel[lbl] = a->a_arrow;
		}
	}
	while (nl > 0 && accel[nl - 1] == -1) {
		nl--;
	}
	for (k = 0; k < nl && accel[k] == -1;) {
		k++;
	}
	if (k < nl) {
		s->s_accel = NEW(int, nl - k);
		if (s->s_accel == NULL) {
			fprintf(stderr, "no mem to add parser accelerators\n");
			exit(1);
		}
		s->s_lower = k;
		s->s_upper = nl;
		for (int i = 0; k < nl; i++, k++) {
			s->s_accel[i] = accel[k];
		}
	}
	DEL(accel);
}

static void
fixdfa(grammar *g, dfa *d)
{
	state *s = d->d_state;

	for (int j = 0; j < d->d_nstates; j++, s++) {
		fixstate(g, d, s);
	}
}

void
addaccelerators(grammar *g)
{
	dfa *d = g->g_dfa;
#ifdef DEBUG
	printf("Adding parser accellerators ...\n");
#endif
	for (int i = g->g_ndfas; --i >= 0; d++) {
		fixdfa(g, d);
    }
	g->g_accel = 1;
#ifdef DEBUG
	printf("Done.\n");
#endif
}
