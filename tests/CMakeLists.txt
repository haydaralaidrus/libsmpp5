set(libsmpp5_test_modules
    pdu
    smpp
    smpp_alert
    smpp_ancillary
    smpp_bind
    smpp_broadcast
    smpp_data
    smpp_multi
    smpp_sm
    smpp_tlv)

foreach(module ${libsmpp5_test_modules})
    add_executable(${module}_test ${module}.c)
    target_link_libraries(${module}_test PRIVATE libsmpp5::smpp5)
    add_test(NAME ${module} COMMAND ${module}_test)
endforeach()
