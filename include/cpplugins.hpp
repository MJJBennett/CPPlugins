#include <utility>

#ifndef CPPLUGINS_CPPLUGINS_HPP
#define CPPLUGINS_CPPLUGINS_HPP

#include <utility>
#include <functional>
#include <iostream>
#include <optional>
#include <map>

#if defined _WIN32
#include <windows.h>
#define CPPLUGINS_WINDOWS 1
#define LIBTYPE HMODULE
#elif defined __APPLE__ || __linux__ || __unix__
#include <dlfcn.h>
#define CPPLUGINS_UNIX 1
#define LIBTYPE void *
#else
#error "Your compiler or operating system is not recognized."
#endif

#define CPPLUGINS_DEBUG 1

namespace cpl {
    std::string dl_path(const std::string& path);

    template<typename f>
    void * make_void(f function_reference);

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
        return static_cast<bool>(hs);
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
#ifdef CPPLUGINS_UNIX
        library_ = dlopen(library_path_.c_str(), RTLD_LAZY);
#elif CPPLUGINS_WINDOWS
        library_ = LoadLibrary(library_path_.c_str());
#endif

        if (!library_) {
            AddState(state_, State::Library_Not_Found);
            return;
        }

        // Attempt to find the 'create' function
#ifdef CPPLUGINS_UNIX
        create_ = reinterpret_cast<create_t> (dlsym(library_, "create"));
#elif CPPLUGINS_WINDOWS
        create_ = reinterpret_cast<create_t> (GetProcAddress(library_, "create"));
#endif

        if (!create_) {
            AddState(state_, State::Create_Not_Found);
            _close_unsafe(library_);
            return;
        }

        // Attempt to find the 'delete' function
#ifdef CPPLUGINS_UNIX
        destroy_ = reinterpret_cast<destroy_t > (dlsym(library_, "destroy"));
#elif CPPLUGINS_WINDOWS
        destroy_ = reinterpret_cast<destroy_t > (GetProcAddress(library_, "destroy"));
#endif

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

    inline void _close_unsafe(LIBTYPE lib) {
#ifdef CPPLUGINS_UNIX
        dlclose(lib);
#elif CPPLUGINS_WINDOWS
        FreeLibrary(lib);
#endif
    }

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
    std::optional<State> state_ = std::nullopt;
    Flag options_ = Flag::None;
    LIBTYPE library_ = nullptr;
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

#undef LIBTYPE // Don't leak macros
#endif //CPPLUGINS_CPPLUGINS_HPP
