add_library(smpp5
    pdu.c
    pdu.h
    smpp.c
    smpp.h)

target_include_directories(smpp5 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})
