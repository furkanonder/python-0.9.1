# Python 0.9.1 - Refactored

This repository is a refactored version of Python 0.9.1. You can access the
original source from [here](https://github.com/smontanaro/python-0.9.1).

## Compile and Build
First, configure the build with:
```sh
cd src
./configure
```
Available configuration options:
- `--with-readline`:  Enable GNU Readline support for enhanced command-line editing
- `--help`:           Display all available configuration options
```sh
./configure
```
Once configuration is complete, build CPython with:
```sh
make
```

## Running
Launch the Python interpreter:
```
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
- Added configure script to simplify build configuration and feature detection.

### Fixed
- Modified ticker variable in `ceval.c` after including `intrcheck.h`.
- Fixed argument passing in `getstrintintarg` function for `getstrarg`.
- Fixed parameter declaration in `reglexec` function in `regexp.c`.
- Refactored `Makefile` for simplicity and improved maintainability.
- Added explicit parentheses to clarify logical operator precedence in `bltinmodule.c`.
- Added explicit parentheses to clarify logical operator precedence in `ceval.c`.
- Added explicit parentheses to clarify logical operator precedence in `regexp.c`.
- Added proper function pointer prototypes in `assign_subscript` function to eliminate deprecated-non-prototype warnings and improve type safety in `ceval.c`.
- Updated signal handler function signature in `fgetsintr.c` to use the correct modern format that includes the signal number parameter.
- Updated signal handler function signature in `timemodule.c` to use the correct modern format that includes the signal number parameter.
- Removed conflicting local declaration of `strchr` function in `regexp.c`.
- Replaced `SIGTYPE` macro with explicit `void` return type for signal handlers to improve clarity and reduce dependencies.

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
- Removed `sigtype.h` and replaced its macro with explicit `void` return type for signal handlers.

## Moved
- Moved license texts from the source code to the `LICENSE` file.
- Moved the `fatal` function from `pythonmain.c` to `errors.c`.