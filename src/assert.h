#ifndef Py_ASSERT_H
#define Py_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#define assert(e) { if (!(e)) { printf("Assertion failed\n"); abort(); } }

#ifdef __cplusplus
}
#endif

#endif /* !Py_ASSERT_H */
