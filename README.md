# MyC

MyC is a small C compiler I am writing to learn more about compilers.

The projects below have served as inspiration:  
https://github.com/DoctorWkt/acwj  
https://github.com/jezze/subc

The rest of the information I needed I got from the textbook:  
_Compilers - Principles, Techniques, and Tools_

## Project Goals
- Be able to compile the SubC compiler
- Produce assembly code for at least two platforms (one being x86,
  next one possibly being a vm I write)
- Implement some machine idependent code optimimizations: constant
  folding, etc.
- Implement a good register allocator
- Produce useful warning and error messages
- Do not rely on another C compiler's runtime (i.e write a crt0)

The project is in its infancy, so expect many sweeping changes.

## Current Features

MyC does not emit actual assembly yet, but emit three address code.

- integer variable declarations (no assignment lists, yet)
- assigments
- +, -, *, /, =, ==, >, <, >=, <=, !, !=, -(unary) operators
- nested statement blocks "{ ... }"

## Next Feature in Progress

Currently working on if statements. Then, I plan to generate actual
X86 assembly.

## Build Instructions

This project is built with CMake.

```
mkdir build
cd build
cmake ..
make
```

## Running

```
./myc [code file]
```
