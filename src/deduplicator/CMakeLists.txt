add_library(dedup SHARED "deduplicator.cpp" "exposed_functions.cpp")
target_link_libraries(dedup ${Boost_LIBRARIES})
target_link_libraries(dedup "-pthread")
#if (DEFINED $ENV{PRIVATE_INCLUDE})
INCLUDE_DIRECTORIES("$ENV{PRIVATE_INCLUDE}")
#endif(DEFINED $ENV{PRIVATE_INCLUDE})

install(TARGETS dedup
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
