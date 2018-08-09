cmake_minimum_required(VERSION 3.10.0)
project(CPPlugins VERSION 0.2.0 DESCRIPTION "An example project using CPPlugins.")
project(ExampleLibrary VERSION 0.1.0 DESCRIPTION "An example library.")

set(CMAKE_CXX_STANDARD 17)
SET(GCC_FUNCTION_EXPOSE "-Wl,-export-dynamic -Wall")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_FUNCTION_EXPOSE}" )

include_directories("${CMAKE_SOURCE_DIR}/include/")

add_executable(CPPlugins
        include/cpplugins.hpp
        main.cpp)

add_library(ExampleLibrary SHARED ${CMAKE_SOURCE_DIR}/test.cpp)

target_link_libraries(CPPlugins dl)