add_library(strace SHARED "strace.cpp" "hookable_functions.cpp")

target_link_libraries(strace ${DYNINST_LIBRARIES})
install(TARGETS strace
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)