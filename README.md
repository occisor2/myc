# MyC

MyC is a small C compiler I am writing to learn more about compilers.

The projects below have served as inspiration:  
https://github.com/DoctorWkt/acwj  
https://github.com/jezze/subc

The rest of the information I needed I got from the textbook:  
_Compilers - Principles, Techniques, and Tools_

## Project Goals
- Be able to compile the SubC compiler
- Produce assembly code for at least two platforms (one being x86)
- Implement some machine idependant code optimimizations: constant
  folding, common sub-expressions, etc.
- Implement a good register allocator
- Produce useful warning and error messages
- Do not rely on another C compiler's runtime (i.e write a crt0)

The project is in its infancy, so expect many sweeping changes.

## Current Features

The latest commits do not produce assembly code but IR code. This is
because I am trying to implement statements as fast as possible, so I
have less refactoring to do later.

- integer variable declarations (no assignment lists, yet)
- assigments
- +, -, *, /, = binary operators with correct precedance.

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
