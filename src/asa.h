/* Interface for asynchronous audio module */

extern int asa_init(void);
extern void asa_done(void);
extern void asa_start_write(char *, int);
extern void asa_start_read(char *, int);
extern int asa_poll(void);
extern int asa_wait(void);
extern int asa_cancel(void);
