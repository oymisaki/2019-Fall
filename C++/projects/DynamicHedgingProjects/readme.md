# Readme

## Environment
Required Libraries and tools:
+ make
+ g++
+ Boost
+ python2.7
+ matplotlib

## Pre Compiling

+ Specify the include directory of python2.7, in my case, it's `-I/usr/include/python2.7` at line 11 of the make file.

+ Ensure boost and python2.7 library is installed in the system default directory. Otherwise, indicate their paths in the `LIB` option in the makefile. Paths shoule be like `<your boost path>/lib`

## Compiling 
Enter project root directory, using the makefile 
  + `make all`

## Run
Under the same directory
  + `make run` or `./bin/main`
