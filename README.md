# Python 0.9.1 - Refactored

This repository is a refactored version of Python 0.9.1. You can access the
original source from [here](https://github.com/smontanaro/python-0.9.1).

## Building and Running
```sh
cd src
make
./python
```

### Added
- `string_dealloc` function added for string objects.
- `float_dealloc` function added for float objects.
- Added void return type to `initregexp` function in `regexpmodule.c`
- Added `intrcheck.h` file.
- Converted K&R style function declarations to ANSI C style.
- Improved function pointer declarations in `object.h` for better readability.
- Cast function pointers in module methods and type objects for type safety.
- Integrated `GNU Readline` support in the `Makefile`.
- Added `GNU Readline` headers in `tokenizer.c`.
- Added header guards and C linkage.

### Fixed
- Modified ticker variable in `ceval.c` after including `intrcheck.h`.
- Fixed argument passing in `getstrintintarg` function for `getstrarg`.
- Fixed parameter declaration in `reglexec` function in `regexp.c`.
- Refactored `Makefile` for simplicity and improved maintainability.

### Removed
- Removed the `shar` directory from the project structure.
- Removed `MS-DOS` specific codes and preprocessor directives.
- Removed `THINK C` specific codes and preprocessor directives.
- Removed the `Amoeba` module(`amoebamodule.c`), along with specific code and preprocessor directives.
- Removed the `Audio` module(`audiomodule.c`), along with specific code and preprocessor directives.
- Removed the `Asynchronous Audio` module(`asa.c`), along with specific code and preprocessor directives.
- Removed the `Panel` module(`panelmodule.c`), along with specific code and preprocessor directives.
- Removed the `GL` specific codes and preprocessor directives.
- Removed the `StdWin` module(`stdwinmodule.c`), along with specific code and preprocessor directives.
- Removed `configmac.c`.
- Removed `BSD_TIME` and `DO_MILLI` preprocessor directives.
- Removed `__STD_C__`, `__STDC__` and `HAVE_STDLIB` preprocessor directives.
- Removed `strdup.c` as its functionality is now provided by the standard library `<string.h>`.
- Removed `strtol.c` as its functionality is now provided by the standard library `<stdlib.h>`.
- Removed `sc_errors.c`, `scdbg.c`, `sc_interpr.c`, `sc_global.h` and `sc_errors.h`.
- Removed `stubcode.h`.
- Removed `rltokenizer.c`.
- Removed `macmodule.c`.
- Removed `getcwd.c`.
- Removed `fmod.c`.
- Removed `HAVE_PROTOTYPES`, `PROTO`, `FPROTO` preprocessor directives and `PROTO.h`.
- Removed `cstubs`.
- Removed `profmain.c`.
- Removed `patchlevel.h`.
- Removed `SYSV` preprocessor directives.
- Removed `initargs`, `initcalls` and `donecalls` functions.
- Removed `allobjects.h`.
- Removed `config.h`.
- Removed custom offsetof implementation from `structmember.h`.
- Removed `strerror.c`.
- Removed `pgenheaders.h`.
- Removed `assert.h` as its functionality is now provided by the standard library `<assert.h>`.

## Moved
- Moved license texts from the source code to the `LICENSE` file.
- Moved the `fatal` function from `pythonmain.c` to `errors.c`.