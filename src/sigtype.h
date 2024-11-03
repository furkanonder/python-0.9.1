#ifndef Py_SIGTYPE_H
#define Py_SIGTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/* The type of signal handlers is somewhat problematic.
   This file encapsulates my knowledge about it:
   It's usually void. Pass -DSIGTYPE=... to cc if you know better. */
#define SIGTYPE void

#endif /* !Py_SIGTYPE_H */
