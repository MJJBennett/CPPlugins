#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <functional>
#include <iostream>
#include <map>

#ifdef _WIN32
#define CPPLUGINS_WINDOWS 1
#elif __APPLE__
#define CPPLUGINS_APPLE 1
#define CPPLUGINS_UNIX 1
#elif __linux__
#define CPPLUGINS_LINUX 1
#define CPPLUGINS_UNIX 1
#elif __unix__
#define CPPLUGINS_UNIX 1
#else
#error "Your compiler or operating system is not recognized."
#endif

#ifdef CPPLUGINS_UNIX
#include <dlfcn.h>
#elif CPPLUGINS_WINDOWS
#include <windows.h>
#endif

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
#ifdef CPPLUGINS_UNIX
        _library = dlopen(_library_path.c_str(), RTLD_LAZY);
#elif CPPLUGINS_WINDOWS
        _library = (void *)LoadLibrary(_library_path.c_str(), RTLD_LAZY);
#endif
        if (!_library) {
            std::cout << "Could not load library " + _library_path + ": \n" << _get_error() << std::endl;
            return;
        }
#ifdef CPPLUGINS_UNIX
        _create = reinterpret_cast<create_t> (dlsym(_library, "create"));
#elif CPPLUGINS_WINDOWS
        _create = reinterpret_cast<create_t> (GetProcAddress(_library, "create"));
#endif
        if (!_create) {
            std::cout << "Error loading create function: " << _get_error() << std::endl;
            _close_unsafe(_library);
            return;
        }
#ifdef CPPLUGINS_UNIX
        _destroy = reinterpret_cast<destroy_t > (dlsym(_library, "destroy"));
#elif CPPLUGINS_WINDOWS
        _destroy = reinterpret_cast<destroy_t > (GetProcAddress(_library, "destroy"));
#endif
        if (!_destroy) {
            std::cout << "Error loading destroy function: " << _get_error() << std::endl;
            _close_unsafe(_library);
            return;
        }
        std::cout << "Successfully loaded plugin library." << std::endl;

        //Now load the API
        api = _create();
        if (!api) {
            std::cout << "Error: Was unable to load API for library." << std::endl;
            _close_unsafe(_library);
        }
    }

    inline bool is_loaded() {
        return _library != nullptr;
    }

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
            _close_unsafe(_library);
        }
    }
private:
    inline void _close_unsafe(void * lib) {
#ifdef CPPLUGINS_UNIX
        dlclose(lib);
#elif CPPLUGINS_WINDOWS
        FreeLibrary(lib);
#endif
    }

    inline std::string _get_error() {
#ifdef CPPLUGINS_UNIX
        return dlerror();
#elif CPPLUGINS_WINDOWS
        return GetLastError();
#endif
    }

    std::map<std::string, void *> _symbols;
    void * _library = nullptr;
    std::string _library_path;
    create_t _create = nullptr;
    destroy_t _destroy = nullptr;
    API_T * api = nullptr;
};

#endif //CPPLUGINS_CPPLUGINS_HPP
