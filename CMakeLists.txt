cmake_minimum_required(VERSION 3.10.0)
project(CPPlugins)

set(CMAKE_CXX_STANDARD 17)
SET(GCC_FUNCTION_EXPOSE "-Wl,-export-dynamic -Wall")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_FUNCTION_EXPOSE}" )

include_directories("${CMAKE_SOURCE_DIR}/include/")

add_executable(CPPlugins
        include/cpplugins.hpp
        main.cpp)

target_link_libraries(CPPlugins dl)