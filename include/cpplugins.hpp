#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <utility>
#include <functional>
#include <iostream>
#include <map>

#if defined _WIN32
#define CPPLUGINS_WINDOWS 1
#include <windows.h>
#elif defined __APPLE__ || __linux__ || __unix__
#define CPPLUGINS_UNIX 1
#include <dlfcn.h>
#else
#error "Your compiler or operating system is not recognized."
#endif

#define CPPLUGINS_DEBUG 1

namespace cpl {
    std::string dl_path(const std::string& path);

    template<typename f>
    void * make_void(f function_reference);
}

namespace cpl {
template<typename API_T>
class Plugin {
public: // Public type definitions.


    // Function used to create an instance of the API
    using create_t = API_T * (*) ();

    using destroy_t = void(*) (API_T*);

public: // Public member functions.

    explicit Plugin(std::string library_path) : library_path_(std::move(library_path)) {}

    inline void load() {
        std::cout << "Loading plugin at " << library_path_ << std::endl;
        if (is_loaded()) {
            std::cout << "Tried to load library " + library_path_ + ", but it is already loaded!\n";
            return;
        }
#ifdef CPPLUGINS_UNIX
        library_ = dlopen(library_path_.c_str(), RTLD_LAZY);
#elif CPPLUGINS_WINDOWS
        library_ = LoadLibrary(library_path_.c_str());
#endif
        if (!library_) {
            std::cout << "Could not load library " + library_path_ + ": \n" << get_error() << std::endl;
            return;
        }
#ifdef CPPLUGINS_UNIX
        create_ = reinterpret_cast<create_t> (dlsym(library_, "create"));
#elif CPPLUGINS_WINDOWS
        create_ = reinterpret_cast<create_t> (GetProcAddress(library_, "create"));
#endif
        if (!create_) {
            std::cout << "Error loading create function: " << get_error() << std::endl;
            _close_unsafe(library_);
            return;
        }
#ifdef CPPLUGINS_UNIX
        destroy_ = reinterpret_cast<destroy_t > (dlsym(library_, "destroy"));
#elif CPPLUGINS_WINDOWS
        destroy_ = reinterpret_cast<destroy_t > (GetProcAddress(library_, "destroy"));
#endif
        if (!destroy_) {
            std::cout << "Error loading destroy function: " << get_error() << std::endl;
            _close_unsafe(library_);
            return;
        }
        std::cout << "Successfully loaded plugin library." << std::endl;

        //Now load the API
        api_ = create_();
        if (!api_) {
            std::cout << "Error: Was unable to load API for library." << std::endl;
            _close_unsafe(library_);
        }

#ifdef CPPLUGINS_DEBUG
        print_state();
#endif
    }

    inline bool is_loaded() {
        return library_ != nullptr;
    }

    API_T * operator->() const {
        return api_;
    }

    ~Plugin() {
        if (api_) {
            if (!library_ || !destroy_) {
                std::cout << "Critical error: API exists but library or destroy function do not!" << std::endl;
            }
            else {
                destroy_(api_);
            }
        }
        if (library_) {
            std::cout << "Closing library." << std::endl;
            _close_unsafe(library_);
        }
    }

#ifdef CPPLUGINS_DEBUG
    inline void print_state() {
        if (!library_ || !create_ || !destroy_ || !api_) std::cout << "State is NULL." << std::endl;
        else std::cout << "Plugin state is stable." << std::endl;
    }
#endif

private: // Private helper functions.

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

private: // Private member variables.

#ifdef CPPLUGINS_UNIX
    void * library_ = nullptr;
#elif CPPLUGINS_WINDOWS
    HMODULE library_ = nullptr;
#endif
    std::string library_path_;
    create_t create_ = nullptr;
    destroy_t destroy_ = nullptr;
    API_T * api_ = nullptr;

}; // End class definition: Plugin
}

std::string cpl::dl_path(const std::string& path) {
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
