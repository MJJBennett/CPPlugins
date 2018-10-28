project(Test1Lib VERSION 0.1.0 DESCRIPTION "A library for a test for basic loading of a library.")
project(Test1Exe VERSION 0.1.0 DESCRIPTION "A test for basic loading of a library.")

# include_directories("${CMAKE_SOURCE_DIR}/include/")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/cpplugins.hpp)

add_library(Test1Lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/BasicLoadingLib.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/BasicLoadingLib.h)

add_executable(Test1Exe BasicLoading.cpp BasicLoadingAPI.h)