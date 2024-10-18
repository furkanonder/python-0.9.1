#define NoBufSize	1
#define TwoBufSize	2
#define StackOverflow	3
#define StackUnderflow	4
#define TypeFailure	5
#define RangeError	6
#define SizeError	7
#define BufferOverflow	8
#define NoEndLoop	9
#define FlagError	10
#define ElementIsNull	11
#define TransError	12

extern object *err_scerr PROTO((int sc_errno));
extern err_scerrset PROTO((int sc_errno, object *value, char *instr));
extern object *StubcodeError;
