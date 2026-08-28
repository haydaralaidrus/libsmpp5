cmake_minimum_required(VERSION 3.15)

project(libsmpp5
    VERSION 0.1.0
    DESCRIPTION "C library for SMPP 5.0"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(CTest)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
