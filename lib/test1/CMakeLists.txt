project(TestLibrary1 VERSION 0.1.0 DESCRIPTION "A library for testing the plugin tool.")

include_directories("${CMAKE_SOURCE_DIR}/include/")

add_library(TestLibrary1 SHARED ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)