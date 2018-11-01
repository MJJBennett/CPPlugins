cmake_minimum_required(VERSION 3.9.0)

add_subdirectory(lib)
add_subdirectory(test)

project(CPPlugins VERSION 1.0.0 DESCRIPTION "An example project using CPPlugins.")

set(CMAKE_CXX_STANDARD 17)
# if(WIN32)
# # SET(GCC_FUNCTION_EXPOSE "--export-all-symbols")
# endif(WIN32)
if(UNIX)
SET(GCC_FUNCTION_EXPOSE "-Wl,-export-dynamic")
endif(UNIX)
SET(GCC_WARNINGS "-Wall -Wextra -pedantic")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS} ${GCC_FUNCTION_EXPOSE}" )

include_directories("${CMAKE_SOURCE_DIR}/include/")

add_executable(CPPlugins
        include/cpplugins.hpp
        example_app.cpp
        test/testing.hpp)

if(WIN32)

endif(WIN32)
if (UNIX)
SET(LINKER_SYSTEM_LIBRARIES "dl")
endif(UNIX)
target_link_libraries(CPPlugins ${LINKER_SYSTEM_LIBRARIES})