#ifndef CPPLUGINS_TESTING_HPP
#define CPPLUGINS_TESTING_HPP

#include <iostream>
#include <string>
#include <optional>

// Very simple 'test' helpers.

template<typename T>
bool test_eq(T lhs, T rhs, std::optional<std::string> test_name = {}) {
    if (lhs == rhs) {
        std::cout << "[PASSED] ";
        if (test_name) std::cout << *test_name << " p";
        else std::cout << 'P';
        std::cout << "assed test." << std::endl;
        return false;
    } else {
        std::cout << "[FAILED] ";
        if (test_name) std::cout << *test_name << " f";
        else std::cout << 'F';
        std::cout << "ailed test (" << __FILE__ << '@' << __LINE__ << ')' << std::endl;
        std::cout << "\t|LHS: '" << lhs << "'|RHS: '" << rhs << "'|" << std::endl;
        return true;
    }
}

template<typename T>
void hard_test_eq(T lhs, T rhs, std::optional<std::string> test_name = {}) {
    if (test_eq(lhs, rhs, test_name)) exit(1);
}

#endif //CPPLUGINS_TESTING_HPP
