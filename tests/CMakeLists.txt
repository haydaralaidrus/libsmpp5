add_executable(pdu_test pdu.c)

target_link_libraries(pdu_test PRIVATE smpp5)

add_test(NAME pdu COMMAND pdu_test)

add_executable(smpp_test smpp.c)

target_link_libraries(smpp_test PRIVATE smpp5)

add_test(NAME smpp COMMAND smpp_test)
