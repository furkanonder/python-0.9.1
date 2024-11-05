/* Bitset primitives used by the parser generator */

#include "pgenheaders.h"
#include "bitset.h"
#include "errors.h"

bitset
newbitset(int nbits)
{
	int nbytes = NBYTES(nbits);
	bitset ss = NEW(BYTE, nbytes);
	
	if (ss == NULL) {
		fatal("no mem for bitset");
    }
	
	ss += nbytes;
	while (--nbytes >= 0) {
		*--ss = 0;
    }
	return ss;
}

void
delbitset(bitset ss)
{
	DEL(ss);
}

int
addbit(bitset ss, int ibit)
{
	int ibyte = BIT2BYTE(ibit);
	BYTE mask = BIT2MASK(ibit);
	
	if (ss[ibyte] & mask) {
		return 0; /* Bit already set */
    }
	ss[ibyte] |= mask;
	return 1;
}

//int
//testbit(bitset ss, int ibit)
//{
//	return (ss[BIT2BYTE(ibit)] & BIT2MASK(ibit)) != 0;
//}

int
samebitset(bitset ss1, bitset ss2, int nbits)
{
	for (int i = NBYTES(nbits); --i >= 0; ) {
		if (*ss1++ != *ss2++) {
			return 0;
        }
    }
	return 1;
}

void
mergebitset(bitset ss1, bitset ss2, int nbits)
{
	for (int i = NBYTES(nbits); --i >= 0; ) {
		*ss1++ |= *ss2++;
    }
}
