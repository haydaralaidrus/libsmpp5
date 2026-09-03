cmake_minimum_required(VERSION 3.15)

project(libsmpp5
    VERSION 1.1.0
    DESCRIPTION "C library for SMPP 5.0"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(CTest)
include(GNUInstallDirs)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# --- Install ---

install(TARGETS smpp5
    EXPORT libsmpp5-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/libsmpp5
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT libsmpp5-targets
    FILE libsmpp5-targets.cmake
    NAMESPACE libsmpp5::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libsmpp5)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/libsmpp5-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/libsmpp5-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libsmpp5)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/libsmpp5-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libsmpp5-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/libsmpp5-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libsmpp5)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/libsmpp5.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libsmpp5.pc
    @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libsmpp5.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# --- Docs ---

find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()
