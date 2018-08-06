#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <functional>
#include <dlfcn.h>
#include <iostream>
#include "../test.h"

std::function<void()> get_logger() {
    std::cout << "Hello from plugin library.\n";
    void * logger_lib = dlopen("./test.so", RTLD_LAZY);
    if (!logger_lib) {
        std::cout << "Error: " << dlerror();
        return []() {};
    }
    test_class (*print)() = reinterpret_cast<test_class (*)()>(dlsym(logger_lib, "print"));

    if (print) {
        std::cout << "Calling print.\n";
        test_class t = (*print)();
        t._print();
    } else {
        std::cout << "Print was null!\n" << dlerror();
    }

    dlclose(logger_lib);
    return []() {};
}

#endif //CPPLUGINS_CPPLUGINS_HPP
