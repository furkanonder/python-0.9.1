This directory contains the source for the Python interpreter.

To build the interpreter, edit the Makefile, follow the instructions
there, and type "make python".

To use the interpreter, you must set the environment variable PYTHONPATH
to point to the directory containing the standard modules.  These are
distributed as a sister directory called 'lib' of this source directory.
Try importing the module 'testall' to see if everything works.

Good Luck!

## TO-DO
- return better errors for file objects (also check read/write allowed, etc.)
- introduce more specific exceptions (e.g., zero divide, index failure, ...)
- why do reads from stdin fail when I suspend the process?
- introduce macros to set/inspect errno for syscalls, to support things
  like getoserr()
- fix interrupt handling (interruptable system calls should call
  intrcheck() to clear the interrupt status)
