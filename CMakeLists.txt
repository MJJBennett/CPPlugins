cmake_minimum_required(VERSION 3.10.0)

add_subdirectory(lib)

project(CPPlugins VERSION 1.0.0 DESCRIPTION "An example project using CPPlugins.")

set(CMAKE_CXX_STANDARD 17)
SET(GCC_FUNCTION_EXPOSE "-Wl,-export-dynamic -Wall")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_FUNCTION_EXPOSE}" )

include_directories("${CMAKE_SOURCE_DIR}/include/")

add_executable(CPPlugins
        include/cpplugins.hpp
        example_app.cpp)

target_link_libraries(CPPlugins PUBLIC dl)