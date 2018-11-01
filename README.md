CPPlugins
---

A very simple single-header wrapper for creating plugins in C++.

Installation
---

Copy `include/cpplugins.hpp` into your project, and use `#include "cpplugins.hpp"` to use it.

Usage
---

Create an API class "`API_T`" that plugins should implement. There is an example API in `include/sample_API.hpp`.

Users that want to create a plugin should write a class that inherits from `API_T`, and also expose two `extern "C"` functions: 

- `API_T * create()` - Should return a pointer to their `API_T` implementation.
- `void destroy(API_T *)`  - Should take a pointer to an object of type `API_T` and `delete` it.

To build a plugin using CMake, use `add_library([Project Name] SHARED [Source files])`. See example in `lib/example1`.

To load a plugin for your application using RAII, create a `cpl::Plugin<API_T, ...Constructor_Ts>` and pass in the path to the library to be loaded, flags (or just `cpl::Flag::None`), and a number of arguments to be made to the constructor equal to the number of types specified with `...Constructor_Ts`. 

Check for state using `is_init` or `get_state`. If the state is good, access the plugin methods using `->`.