/* The type of signal handlers is somewhat problematic.
   This file encapsulates my knowledge about it:
   - on the Mac (THINK C), it's int for 3.0, void for 4.0
   - on other systems, it's usually void, except it's int on vax Ultrix.
   Pass -DSIGTYPE=... to cc if you know better. */

#ifndef SIGTYPE

#if defined(vax) && !defined(AMOEBA)
#define SIGTYPE int
#else
#define SIGTYPE void
#endif

#endif /* !SIGTYPE */
