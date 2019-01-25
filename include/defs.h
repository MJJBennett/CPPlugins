#ifndef CPPLUGINS_HEADER_INCLUDES
#define CPPLUGINS_HEADER_INCLUDES

#include <utility>
#include <functional>
#include <optional>
#include <map>

#include <iostream>

#ifdef LOAD_LIBRARY
#define LOAD_LIBRARY_OLD LOAD_LIBRARY
#undef LOAD_LIBRARY
#endif
#ifdef LOAD_FUNCTION
#define LOAD_FUNCTION_OLD LOAD_FUNCTION
#undef LOAD_FUNCTION
#endif

#if defined _WIN32
// Windows includes
#include <windows.h>
// Windows functions
#define CPPLUGINS_OS_WINDOWS 1
#define CPPLUGINS_LIBRARY_TYPE HMODULE
#define LOAD_LIBRARY(x) LoadLibrary(x)
#define LOAD_FUNCTION(x, y) GetProcAddress(x, y)
#define CLOSE_LIBRARY(x) FreeLibrary(x)
#define DLL_EXT ".dll"

#elif defined __APPLE__ || __linux__ || __unix__
// POSIX includes
#include <dlfcn.h>
// POSIX functions
#define OS_UNIX 1
#define LIBRARY_TYPE void *
#define LOAD_LIBRARY(x) dlopen(x, RTLD_LAZY)
#define LOAD_FUNCTION(x, y) dlsym(x, y)
#define CLOSE_LIBRARY(x) dlclose(x)
#define DLL_EXT ".so"

#else
#error "Your compiler or operating system is not recognized."
#endif

#endif //CPPLUGINS_HEADER_INCLUDES
