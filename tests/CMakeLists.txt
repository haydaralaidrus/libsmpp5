add_executable(pdu_test pdu.c)

target_link_libraries(pdu_test PRIVATE smpp5)

add_test(NAME pdu COMMAND pdu_test)
