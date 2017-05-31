add_library(strace SHARED "strace.cpp")
target_link_libraries(strace ${DYNINST_LIBRARIES})
