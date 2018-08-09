#include <iostream>
#include "cpplugins.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Hello from main.\n";
    get_logger();
    return 0;
}

void do_something_else() {
    std::cout << "something else!!\n";
}