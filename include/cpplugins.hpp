#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <functional>
#include <dlfcn.h>
#include <iostream>
#include "../test.h"
#include "sample_interface.hpp"

std::function<void()> get_logger() {
    std::cout << "Hello from plugin library.\n";
    void * logger_lib = dlopen("./test.so", RTLD_LAZY);
    if (!logger_lib) {
        std::cout << "Error: " << dlerror();
        return []() {};
    }
    using create = sample_interface * (*) ();
    using destroy = void(*) (sample_interface*);

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

    sample_interface *print = create_();

    if (print) {
        std::cout << "Calling print.\n";
        sample_interface * t = create_();
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
