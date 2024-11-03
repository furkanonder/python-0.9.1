/* Error handling definitions */
void err_set(object *);
void err_setval(object *, object *);
void err_setstr(object *, char *);
int err_occurred(void);
void err_get(object **, object **);
void err_clear(void);

/* Predefined exceptions */
extern object *RuntimeError;
extern object *EOFError;
extern object *TypeError;
extern object *MemoryError;
extern object *NameError;
extern object *SystemError;
extern object *KeyboardInterrupt;

/* Some more planned for the future */
#define IndexError		    RuntimeError
#define KeyError		    RuntimeError
#define ZeroDivisionError	RuntimeError
#define OverflowError		RuntimeError

/* Convenience functions */
extern int err_badarg(void);
extern object *err_nomem(void);
extern object *err_errno(object *);
extern void err_input(int);
extern void err_badcall(void);
