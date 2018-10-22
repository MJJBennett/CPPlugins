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

void sample_implementation::init() {
    _ID = "Sample_Plugin_1" + ((check_version()) ? std::string("_UpToDate"): std::string("_Outdated"));
}

bool sample_implementation::check_version() {
    using get_version_t = int (*) ();
    std::cout << "Checking application version." << std::endl;
    if (!_symbols.count("get_application_version")) {
        std::cout << "Application version retrieval not supported!\n";
        return false;
    } 
    int version = reinterpret_cast<get_version_t>(_symbols.at("get_application_version"))();
    if (version > 3) {
        std::cout << "Application version is " << version << "; this plugin may not be supported." << std::endl;
        return false;
    }
    else {
        std::cout << "Application version is " << version << "; this plugin is supported." << std::endl;
        return true;
    }
}

int sample_implementation::process_data(int data) {
    return data + 5;
}

std::string sample_implementation::get_id() {
    return _ID;
}

int sample_implementation::add(int a, int b) {
    return a + b;
}
