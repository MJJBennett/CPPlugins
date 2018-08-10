#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <functional>
#include <dlfcn.h>
#include <iostream>
#include "../lib/example1/test.h"
#include "sample_API.hpp"
#include <map>

template<typename API_T>
class Plugin {
public:
    using create_t = API_T * (*) ();
    using destroy_t = void(*) (API_T*);

    explicit Plugin(std::string library_path) : _library_path(library_path) {}

    inline void load() {
        if (is_loaded()) {
            std::cout << "Tried to load library " + _library_path + ", but it is already loaded!\n";
            return;
        }
        _library = dlopen(_library_path.c_str(), RTLD_LAZY);
        if (!_library) {
            std::cout << "Could not load library " + _library_path + ": \n" << dlerror() << std::endl;
            return;
        }
        _create = reinterpret_cast<create_t> (dlsym(_library, "create"));
        if (!_create) {
            std::cout << "Error loading create function: " << dlerror() << std::endl;
            dlclose(_library);
            return;
        }
        _destroy = reinterpret_cast<destroy_t > (dlsym(_library, "destroy"));
        if (!_destroy) {
            std::cout << "Error loading destroy function: " << dlerror() << std::endl;
            dlclose(_library);
            return;
        }
        std::cout << "Successfully loaded plugin library." << std::endl;

        //Now load the API
        api = _create();
        if (!api) {
            std::cout << "Error: Was unable to load API for library." << std::endl;
            dlclose(_library);
        }
    }

    inline bool is_loaded() {
        return _library != nullptr;
    }

//    inline bool set_safemode(bool mode) {
//        _safemode = mode;
//    }

    API_T * operator->() const {
        return api;
    }

    ~Plugin() {
        if (api) {
            if (!_library || !_destroy) {
                std::cout << "Critical error: API exists but library or destroy function do not!" << std::endl;
            }
            else {
                _destroy(api);
            }
        }
        if (_library) {
            std::cout << "Closing library." << std::endl;
            dlclose(_library);
        }
    }
private:
    std::map<std::string, void *> _symbols;
    void * _library = nullptr;
    std::string _library_path;
    create_t _create = nullptr;
    destroy_t _destroy = nullptr;
    API_T * api = nullptr;
    //bool _safemode = true;
};

#endif //CPPLUGINS_CPPLUGINS_HPP
