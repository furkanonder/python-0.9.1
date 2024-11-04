#ifndef Py_BITSET_H
#define Py_BITSET_H

#ifdef __cplusplus
extern "C" {
#endif

#define BYTE    char
typedef BYTE    *bitset;

bitset newbitset(int nbits);
void delbitset(bitset bs);
/* int testbit(bitset bs, int ibit); Now a macro, see below */
int addbit(bitset bs, int ibit);     /* Returns 0 if already set */
int samebitset(bitset bs1, bitset bs2, int nbits);
void mergebitset(bitset bs1, bitset bs2, int nbits);

#define BITSPERBYTE    	(8 * sizeof(BYTE))
#define NBYTES(nbits)	(((nbits) + BITSPERBYTE - 1) / BITSPERBYTE)

#define BIT2BYTE(ibit)	((ibit) / BITSPERBYTE)
#define BIT2SHIFT(ibit)	((ibit) % BITSPERBYTE)
#define BIT2MASK(ibit)	(1 << BIT2SHIFT(ibit))
#define BYTE2BIT(ibyte)	((ibyte) * BITSPERBYTE)

#define testbit(ss, ibit) (((ss)[BIT2BYTE(ibit)] & BIT2MASK(ibit)) != 0)

#ifdef __cplusplus
}
#endif

#endif /* !Py_BITSET_H */
