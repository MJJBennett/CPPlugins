#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <utility>
#include <functional>
#include <iostream>
#include <optional>
#include <map>

#ifdef LOAD_LIBRARY
#warning LOAD_LIBRARY macro being redefined.
#endif
#ifdef LOAD_FUNCTION
#warning LOAD_FUNCTION macro being redefined.
#endif

#if defined _WIN32
// Windows includes
#include <windows.h>
// Windows functions
#define OS_WINDOWS 1
#define LIBRARY_TYPE HMODULE
#define LOAD_LIBRARY(x) LoadLibrary(x)
#define LOAD_FUNCTION(x, y) GetProcAddress(x, y)

#elif defined __APPLE__ || __linux__ || __unix__
// POSIX includes
#include <dlfcn.h>
// POSIX functions
#define OS_UNIX 1
#define LIBRARY_TYPE void *
#define LOAD_LIBRARY(x) dlopen(x, RTLD_LAZY)
#define LOAD_FUNCTION(x, y) dlsym(x, y)

#else
#error "Your compiler or operating system is not recognized."
#endif

namespace cpl {
    std::string dl_path(const std::string &path);

    template<typename f>
    void *make_void(f function_reference);

    enum class Flag : uint8_t {
        None = 0,
        Late_Load = 1,
        All = 255
    };

    enum class State : uint32_t {
        Success = 0,
        Fail = 1,
        Library_Not_Found = 3,
        Create_Not_Found = 5,
        Destroy_Not_Found = 9,
        API_Not_Found = 17,
    };

    Flag operator&(Flag lhs, Flag rhs) {
        return static_cast<Flag>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }

    bool operator!(Flag hs) {
        return !static_cast<bool>(hs);
    }

    void AddState(std::optional<State> &state, State flag) {
        if (!state) {
            state = State{flag};
        } else {
            state = static_cast<State>(static_cast<uint32_t>(*state) & static_cast<uint32_t>(flag));
        }
    }
}

namespace cpl {
template<typename API_T>
class Plugin {
    // Function used to create an instance of the API
    using create_t = API_T * (*) ();

    // Function used to destroy an instance of the API
    using destroy_t = void(*) (API_T*);

public: // Public member functions.

    // Constructor
    explicit Plugin(std::string library_path, std::optional<State> state, Flag options = Flag::None) :
            state_(std::move(state)), options_(options), library_path_(std::move(library_path)) {
        if (!(options_ & Flag::Late_Load))
            load();
    }

    // Load
    void load() {
        // Attempt to open the library
        library_ = LOAD_LIBRARY(library_path_.c_str());

        if (!library_) {
            AddState(state_, State::Library_Not_Found);
            return;
        }

        // Attempt to find the 'create' function
        create_ = reinterpret_cast<create_t> (LOAD_FUNCTION(library_, "create"));

        if (!create_) {
            AddState(state_, State::Create_Not_Found);
            _close_unsafe(library_);
            return;
        }

        // Attempt to find the 'delete' function
        destroy_ = reinterpret_cast<destroy_t > (LOAD_FUNCTION(library_, "destroy"));

        if (!destroy_) {
            AddState(state_, State::Destroy_Not_Found);
            _close_unsafe(library_);
            return;
        }

        //Now, load the API
        api_ = create_();
        if (!api_) {
            AddState(state_, State::API_Not_Found);
            _close_unsafe(library_);
            return;
        }
    }

    API_T * operator->() const {
        return api_;
    }

    ~Plugin() {
        if (library_) {
            if (api_ && destroy_) {
                destroy_(api_);
            }
            _close_unsafe(library_);
        }
    }

    inline bool is_init() {
        return library_ && create_ && destroy_ && api_;
    }

private: // Private helper functions.

    inline void _close_unsafe(LIBRARY_TYPE  lib) {
#ifdef OS_UNIX
        dlclose(lib);
#elif OS_WINDOWS
        FreeLibrary(lib);
#endif
    }

    inline std::string get_error() {
#ifdef OS_UNIX
        return dlerror();
#elif OS_WINDOWS
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
    std::optional<State> state_ = std::nullopt;
    Flag options_ = Flag::None;
    LIBRARY_TYPE  library_ = nullptr;
    std::string library_path_;
    create_t create_ = nullptr;
    destroy_t destroy_ = nullptr;
    API_T * api_ = nullptr;

}; // End class definition: Plugin
}

std::string cpl::dl_path(const std::string& path) {
#ifdef OS_WINDOWS
    return path + ".dll";
#elif OS_UNIX
    return path + ".so";
#endif
}

template<typename f>
void * cpl::make_void(f function_reference) {
    return (void *)function_reference;
}

// Don't leak macros
#undef OS_WINDOWS
#undef OS_UNIX
#undef LIBRARY_TYPE
#undef LOAD_LIBRARY
#undef LOAD_FUNCTION
#endif //CPPLUGINS_CPPLUGINS_HPP
