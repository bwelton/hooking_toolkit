add_library(dedup SHARED "deduplicator.cpp" "exposed_functions.cpp")
target_link_libraries(dedup ${Boost_LIBRARIES})
target_link_libraries(dedup "-pthread")
install(TARGETS dedup
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)