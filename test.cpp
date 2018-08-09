#include <iostream>
#include <string>
#include <vector>
#include "test.h"

//test_class::test_class() {
//    std::cout << "Initialized a test class.\n";
//}
//
//void test_class::_print() {
//    std::cout << "Printed with test class.\n";
//}
//
//test_class print() {
//    std::cout << "Hello from test.cpp!" << std::endl;
//    std::vector<int> nums{1,2,3};
//    for (auto&& n : nums) std::cout << n << std::endl;
//}

sample_interface * get() {
    return new sample_implementation;
}

void destroy(sample_interface * s) {
    delete s;
}

//extern void print(int, int) {
//    std::cout << "Hello from test.cpp!" << std::endl;
//}
void sample_implementation::do_something() {
    std::cout << "Doing something..." << std::endl;
    private_do_something();
}

int sample_implementation::get_something() {
    return 5;
}

void sample_implementation::private_do_something() {
    std::cout << "Did it!\n";
}
