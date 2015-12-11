CMinor
======

##About

A project to create a compiled (x86\_64) C-like language using the Flex scanner-generator and the Bison parser-generator.
The spec for the language is located in the `spec` directory. It is subject to change.

See the `tests/codegen/other` directory for examples of generated codes (with binaries compiled on RHEL).

##Install
Building this project requires gcc or clang, Flex, Bison, and ld (for linking to compiled C programs).

It has been tested on the OSX and RHEL gcc (gcc in OSX links to clang), Flex, and Bison.

* The codegen-debug and linking with library (`ast/library.c`) option currently only works on linux
* May work with but does not support Windows (or other unknown systems) C compilation.

##Build
Build the project with `make`.

##Run
Run an input file through scanning, parsing, name resolution, typechecking, and codegen with `cminor -codegen [input file] [optional output file]`.

* More details by running `cminor --help`.

