# myc

Myc is a small C compiler I am writing to learn more about compilers.
I have gathered information from a few sources, but my main guide for
this project is this project on Github:
https://github.com/DoctorWkt/acwj

The linked project is written in C, but I have decided to write mine
in C++ out of personal preference. This means my project is structured
a little differently, but it uses the same ideas with a few slight
detours.

## Build Instructions

This project is built with CMake.

```
mkdir build
cd build
cmake ..
make
```

## Running

Right now the compiler just takes a source file name. The resulting
assembly code is automatically assembled and linked producing an a.out
file. I am planning on adding more command line options soon.

```
./myc [code file]
```

## Tests

I plan to write tests in the future. The project is setup with GTest,
but no tests have been implemented yet.
