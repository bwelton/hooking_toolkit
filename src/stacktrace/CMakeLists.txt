add_library(strace SHARED "strace.cpp" "hookable_functions.cpp")
add_library(strace_third_party SHARED "strace_third_party.cpp" "hook_func_third_party.cpp")

add_library(simple_hook SHARED "SimpleHooks.cpp")
target_link_libraries(strace ${DYNINST_LIBRARIES})
target_link_libraries(strace ${Boost_LIBRARIES})
target_link_libraries(strace "-pthread")
# target_link_libraries(strace_third_party ${DYNINST_LIBRARIES})
target_link_libraries(strace_third_party "-Wl,--as-needed")
#target_link_libraries(strace_third_party ${Boost_LIBRARIES})
add_executable(strace_launcher "launcher.cpp" "StraceThirdPartyLauncher.cpp")
target_link_libraries(strace_launcher ${DYNINST_LIBRARY_DIR})
target_link_libraries(strace_launcher ${DYNINST_LIBRARY_API})
# target_link_libraries(strace_launcher strace_third_party)
target_link_libraries(strace_launcher "-Wl,--as-needed")
install(TARGETS strace strace_third_party strace_launcher simple_hook
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
