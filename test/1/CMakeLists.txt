project(Test1Lib VERSION 0.1.0 DESCRIPTION "A library for a test for basic loading of a library.")
project(Test1Exe VERSION 0.1.0 DESCRIPTION "A test for basic loading of a library.")

# include_directories("${CMAKE_SOURCE_DIR}/include/")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/)

if(UNIX)
    SET(GCC_FUNCTION_EXPOSE "-Wl,-export-dynamic")
endif(UNIX)
SET(GCC_WARNINGS "-Wall -Wextra -pedantic")

if (UNIX)
    SET(LINKER_SYSTEM_LIBRARIES "dl")
endif(UNIX)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS} ${GCC_FUNCTION_EXPOSE}" )

add_library(Test1Lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/BasicLoadingLib.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/BasicLoadingLib.h)

add_executable(Test1Exe BasicLoading.cpp BasicLoadingAPI.h)
target_link_libraries(Test1Exe ${LINKER_SYSTEM_LIBRARIES})