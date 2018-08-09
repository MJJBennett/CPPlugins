project(ExampleLibrary VERSION 0.1.0 DESCRIPTION "An example library.")

include_directories("${CMAKE_SOURCE_DIR}/include/")

add_library(ExampleLibrary SHARED ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)