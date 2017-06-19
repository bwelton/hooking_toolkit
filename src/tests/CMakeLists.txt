add_executable(basic_test "basic_test.cpp")
add_library(basic_replacelib SHARED "basic_replacelib.cpp")

#target_link_libraries(basic_test ${DYNINST_LIBRARIES})
install(TARGETS basic_test basic_replacelib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

