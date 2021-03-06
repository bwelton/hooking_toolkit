cmake_minimum_required (VERSION 2.8.11)
SET(CMAKE_CXX_STANDARD 11)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
project(hooking_toolkit)
set(BOOST_ROOT ${BOOST_ROOT})
find_package(Boost COMPONENTS system thread REQUIRED)
find_package(PythonLibs)
#find_package(Dyninst COMPONENTS stackwalker)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES("${CMAKE_INSTALL_PREFIX}/include")
INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")

if (NOT DYNINST_INSTALL_DIR)
	ExternalProject_Add(
	  dyninst
	  GIT_REPOSITORY "https://github.com/bwelton/dyninst.git"
	  #GIT_REPOSITORY "https://github.com/dyninst/dyninst.git"
          #GIT_TAG "a8252fd9ace7dd837f98b0db750c588560feea95"
	  SOURCE_DIR "${PROJECT_BINARY_DIR}/3rdparty/dyninst"
	  CMAKE_ARGS -DLIBDWARF_LIBRARIES=/sw/xk6/libdwarf/20130729-b/cle5.2_gnu4.8.2/lib/libdwarf.so -DLIBDWARF_INCLUDE_DIR=/sw/xk6/libdwarf/20130729-b/cle5.2_gnu4.8.2/include -DLIBELF_LIBRARIES=${CMAKE_SOURCE_DIR}/build/libelf_tmp/lib/libelf_pic.a -DLIBELF_INCLUDE_DIR=${CMAKE_SOURCE_DIR}/build/libelf_tmp/include -DBOOST_ROOT=${BOOST_ROOT} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	  INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
	  BUILD_COMMAND $(MAKE) -j 32
	  UPDATE_COMMAND ""
	)  
	set(DYNINST_LIBRARY_DIR "-L${CMAKE_INSTALL_PREFIX}/lib")
else()
	set(DYNINST_LIBRARY_DIR "-L${DYNINST_INSTALL_DIR}/lib")
	INCLUDE_DIRECTORIES("${DYNINST_INSTALL_DIR}/include")
endif()

set(DYNINST_LIBRARY_COMMON  -lcommon)
set(DYNINST_LIBRARY_DYNC  -ldynC_API)
set(DYNINST_LIBRARY_DWARF  -ldynDwarf)
set(DYNINST_LIBRARY_ELF  -ldynElf)
set(DYNINST_LIBRARY_API_RT  -ldyninstAPI_RT)
set(DYNINST_LIBRARY_API  -ldyninstAPI)
set(DYNINST_LIBRARY_INSTAPI  -linstructionAPI)
set(DYNINST_LIBRARY_PARSE  -lparseAPI)
set(DYNINST_LIBRARY_PATCH  -lpatchAPI)
set(DYNINST_LIBRARY_PCONTROL  -lpcontrol)
set(DYNINST_LIBRARY_STACKWALK  -lstackwalk)
set(DYNINST_LIBRARY_SYMLITE  -lsymLite)
set(DYNINST_LIBRARY_SYMTAB  -lsymtabAPI)
set(DYNINST_LIBRARIES "${DYNINST_LIBRARY_DIR} ${DYNINST_LIBRARY_COMMON} ${DYNINST_LIBRARY_DYNC} ${DYNINST_LIBRARY_DWARF} ${DYNINST_LIBRARY_ELF} ${DYNINST_LIBRARY_API_RT} ${DYNINST_LIBRARY_API} ${DYNINST_LIBRARY_INSTAPI} ${DYNINST_LIBRARY_PARSE} ${DYNINST_LIBRARY_PATCH} ${DYNINST_LIBRARY_PCONTROL} ${DYNINST_LIBRARY_STACKWALK} ${DYNINST_LIBRARY_SYMLITE} ${DYNINST_LIBRARY_SYMTAB}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed")
message(${DYNINST_LIBRARIES})
add_subdirectory(private)
add_subdirectory(src/stacktrace)
add_subdirectory(src/inst_lib)
add_subdirectory(src/tests)
add_subdirectory(src/python_wrapper)
add_subdirectory(src/scripts)
add_subdirectory(src/examples)
add_subdirectory(src/deduplicator)
