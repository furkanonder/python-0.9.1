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
- Added `intrcheck.h` file
- Added `config.h` file

### Fixed
- Modified ticker variable in `ceval.c` after including `intrcheck.h`
- Fixed argument passing in `getstrintintarg` function for `getstrarg`
- Fixed parameter declaration in `reglexec` function in `regexp.c`

### Removed
- Deleted the `shar` directory from the project structure.

## Moved
- Moved license texts from the source code to the `LICENSE` file.