add_executable(basic_test "basic_test.cpp")

target_link_libraries(basic_test ${DYNINST_LIBRARIES})
install(TARGETS basic_test
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)