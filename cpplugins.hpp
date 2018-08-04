#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <functional>
#include <dlfcn.h>
#include <iostream>

std::function<void()> get_logger() {
    void * logger_lib = dlopen("./test.so", RTLD_LAZY);
    if (!logger_lib) {
        std::cout << "Error: " << dlerror();
        return []() {};
    }
    void (*print)() = (void (*)())dlsym(logger_lib, "print");

    if (print) {
        print();
    }

    dlclose(logger_lib);
    return []() {};
}

#endif //CPPLUGINS_CPPLUGINS_HPP
