#include <iostream>
#include <string>
#include <vector>
#include "test.h"

sample_API * create() {
    return new sample_implementation;
}

void destroy(sample_API * s) {
    delete s;
}

bool sample_implementation::check_version() {
    std::cout << "Checking application version." << std::endl;
    int version = get_application_version();
    if (version > 3) {
        std::cout << "Application version is " << version << "; it may not be supported." << std::endl;
        return false;
    }
    else {
        std::cout << "Application version is " << version << "; it is supported." << std::endl;
        return true;
    }
}

int sample_implementation::process_data(int data) {
    return data + 5;
}

std::string sample_implementation::get_id() {
    return _ID;
}
