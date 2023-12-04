# myc

Myc is a small C compiler I am writing to learn more about compilers.

## Build Instructions

This project is built with CMake.

```
mkdir build
cd build
cmake ..
make
```

## Running

Right now the compiler just works on one source file.

```
./myc [code file]
```

## Tests

I plan to write tests in the future. The project is setup with GTest,
but no tests have been implemented yet.
