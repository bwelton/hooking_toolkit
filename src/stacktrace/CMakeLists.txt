add_library(strace SHARED "strace.cpp" "hookable_functions.cpp")

target_link_libraries(strace ${DYNINST_LIBRARIES})
