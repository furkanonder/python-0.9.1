#ifndef Py_REGMAGIC_H
#define Py_REGMAGIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* The first byte of the regexp internal "program" is actually this magic
 * number; the start node begins in the second byte. */
#define	MAGIC	0234

#ifdef __cplusplus
}
#endif

#endif /* !Py_REGMAGIC_H */
