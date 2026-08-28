# libsmpp5
Library for SMPP 5.0 Protocol, as specified by the SMPP Developers Forum.

## Prerequisites
- CMake >=3.15
- GCC or Clang (C11-capable)

## Compiling
```sh
cmake -S . -B build
cmake --build build
```

The test suite is wired into CTest and can be run with:
```sh
ctest --test-dir build
```

## Installation
There is no `cmake --install` target yet. For now, consume libsmpp5 by
adding this repository as a CMake subdirectory and linking against the
`smpp5` target:
```cmake
add_subdirectory(path/to/libsmpp5)
target_link_libraries(your_target PRIVATE smpp5)
```
