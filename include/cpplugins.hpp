#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <functional>
#include <dlfcn.h>
#include <iostream>
#include "../lib/example1/test.h"
#include "sample_API.hpp"

std::function<void()> get_logger() {
    std::cout << "Hello from plugin library.\n";
    void * logger_lib = dlopen("./libExampleLibrary.so", RTLD_LAZY);
    if (!logger_lib) {
        std::cout << "Error: " << dlerror();
        return []() {};
    }
    using create = sample_API * (*) ();
    using destroy = void(*) (sample_API*);

    auto create_ = reinterpret_cast<create> (dlsym(logger_lib, "get"));
    if (!create_) {
        std::cout << "Error: " << dlerror();
        return []() {};
    }

    auto destroy_ = reinterpret_cast<destroy>  (dlsym(logger_lib, "destroy"));
    if (!destroy_) {
        std::cout << "Error: " << dlerror();
        return []() {};
    }

    sample_API *print = create_();

    if (print) {
        std::cout << "Calling print.\n";
        sample_API * t = create_();
        t->do_something();
        std::cout << t->get_something() << std::endl;
        destroy_(print);
    } else {
        std::cout << "Print was null!\n" << dlerror();
    }

    dlclose(logger_lib);
    return []() {};
}

#endif //CPPLUGINS_CPPLUGINS_HPP
