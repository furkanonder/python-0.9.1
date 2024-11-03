/* The type of signal handlers is somewhat problematic.
   This file encapsulates my knowledge about it:
   It's usually void. Pass -DSIGTYPE=... to cc if you know better. */

#define SIGTYPE void
