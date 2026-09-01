# libsmpp5

Library for SMPP 5.0 Protocol, as specified by the SMPP Developers Forum.

## Prerequisites

- CMake >=3.15
- GCC or Clang (C11-capable)
- Doxygen (Optional, for generating docs.)

## Compiling

```sh
cmake -S . -B build
cmake --build build
```

The test suite is using CTest:

```sh
ctest --test-dir build
```

## Documentation

If [Doxygen](https://www.doxygen.org/) is installed:

```sh
cmake --build build --target docs
```

Output goes to `build/docs/html/index.html`.

## Installation

Build and install like any CMake project:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix /usr/local
```

Pass `-DBUILD_SHARED_LIBS=ON` to install a versioned `.so` instead of a
static archive.

## Usage

Headers are namespaced under `libsmpp5/`:

```c
#include <libsmpp5/pdu.h>
#include <libsmpp5/smpp.h>
...
```

Via CMake:

```cmake
find_package(libsmpp5 REQUIRED)
target_link_libraries(your_target PRIVATE libsmpp5::smpp5)
```

Via `pkg-config`:

```sh
cc $(pkg-config --cflags libsmpp5) your_app.c $(pkg-config --libs libsmpp5)
```

As a CMake subdirectory:

```cmake
add_subdirectory(path/to/libsmpp5)
target_link_libraries(your_target PRIVATE libsmpp5::smpp5)
```
