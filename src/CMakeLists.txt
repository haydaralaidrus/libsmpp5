add_library(smpp5
    pdu.c
    smpp.c
    smpp_tlv.c
    ${PROJECT_SOURCE_DIR}/include/libsmpp5/pdu.h
    ${PROJECT_SOURCE_DIR}/include/libsmpp5/smpp.h
    ${PROJECT_SOURCE_DIR}/include/libsmpp5/smpp_tlv.h)

add_library(libsmpp5::smpp5 ALIAS smpp5)

target_include_directories(smpp5 PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(smpp5 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})
