add_library(inst_lib SHARED "inst.cpp")

target_link_libraries(inst_lib ${DYNINST_LIBRARIES})
install(TARGETS inst_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)