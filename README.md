# myc

Myc is a small C compiler I am writing to learn more about compilers.

The projects below have served as inspiration:  
https://github.com/DoctorWkt/acwj  
https://github.com/jezze/subc

The example projects I used were very simple compared to my goals. Myc
generates 3 address code before generating any machine dependant code.
I plan to use this to implement many different optimization
algorithms.

The project is in its infancy, so expect many sweeping changes.

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
