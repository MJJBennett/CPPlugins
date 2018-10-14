#include <utility>

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

#define CPPLUGINS_DEBUG 1

namespace cpl {
    std::string dl_path(const std::string& path);

    template<typename f>
    void * make_void(f function_reference);
}

namespace cpl {
template<typename API_T, typename log_t = void>
class Plugin {
public:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public type definitions.

    // Function used to create an instance of the API
    using create_t = API_T * (*) ();

    using destroy_t = void(*) (API_T*);

public:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public member functions.

    explicit Plugin(std::string library_path) : library_path_(std::move(library_path)) {}

    explicit Plugin(std::string library_path, typename std::enable_if<!std::is_void_v<log_t>> logger) :
        library_path_(std::move(library_path), logger_(logger)) {}

    inline void load() {
        std::cout << "Loading plugin at " << library_path_ << std::endl;
        if (is_loaded()) {
            std::cout << "Tried to load library " + library_path_ + ", but it is already loaded!\n";
            return;
        }
#ifdef CPPLUGINS_UNIX
        _library = dlopen(library_path_.c_str(), RTLD_LAZY);
#elif CPPLUGINS_WINDOWS
        _library = LoadLibrary(library_path_.c_str());
#endif
        if (!_library) {
            std::cout << "Could not load library " + library_path_ + ": \n" << get_error() << std::endl;
            return;
        }
#ifdef CPPLUGINS_UNIX
        _create = reinterpret_cast<create_t> (dlsym(_library, "create"));
#elif CPPLUGINS_WINDOWS
        _create = reinterpret_cast<create_t> (GetProcAddress(_library, "create"));
#endif
        if (!_create) {
            std::cout << "Error loading create function: " << get_error() << std::endl;
            _close_unsafe(_library);
            return;
        }
#ifdef CPPLUGINS_UNIX
        _destroy = reinterpret_cast<destroy_t > (dlsym(_library, "destroy"));
#elif CPPLUGINS_WINDOWS
        _destroy = reinterpret_cast<destroy_t > (GetProcAddress(_library, "destroy"));
#endif
        if (!_destroy) {
            std::cout << "Error loading destroy function: " << get_error() << std::endl;
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

#ifdef CPPLUGINS_DEBUG
        print_state();
#endif
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

#ifdef CPPLUGINS_DEBUG
    inline void print_state() {
        if (!_library || !_create || !_destroy || !api) std::cout << "State is NULL." << std::endl;
        else std::cout << "Plugin state is stable." << std::endl;
    }
#endif

private:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private helper functions.

#ifdef CPPLUGINS_UNIX
    inline void _close_unsafe(void * lib) {
        std::cout << "Warning: Closing library for plugin " + library_path_;
        dlclose(lib);
    }
#elif CPPLUGINS_WINDOWS
    inline void _close_unsafe(HMODULE lib) {
        std::cout << "Warning: Closing library for plugin " << library_path_ << std::endl;
        FreeLibrary(lib);
    }
#endif

    inline std::string get_error() {
#ifdef CPPLUGINS_UNIX
        return dlerror();
#elif CPPLUGINS_WINDOWS
        //The following code is taken directly from Jamin Grey's answer here:
        //https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror
        //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
        //Get the error message, if any.
        DWORD errorMessageID = ::GetLastError();
        if(errorMessageID == 0)
            return std::string(); //No error message has been recorded

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string message(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);

        return message;
#endif
    }

private:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private member variables.

#ifdef CPPLUGINS_UNIX
    void * _library = nullptr;
#elif CPPLUGINS_WINDOWS
    HMODULE _library = nullptr;
#endif
    std::string library_path_;
    create_t _create = nullptr;
    destroy_t _destroy = nullptr;
    API_T * api = nullptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End class definition: Plugin
};
}

std::string cpl::dl_path(const std::string& path) {
    //Temporary solution for now
#ifdef CPPLUGINS_WINDOWS
    return path + ".dll";
#elif CPPLUGINS_UNIX
    return path + ".so";
#endif
}

template<typename f>
void * cpl::make_void(f function_reference) {
    return (void *)function_reference;
}

#endif //CPPLUGINS_CPPLUGINS_HPP
