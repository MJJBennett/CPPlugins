#include <iostream>
#include "sample_API.hpp"
#include "cpplugins.hpp"

int main(int argc, char* argv[]) {
    Plugin<sample_API> plugin("./lib/example1/libExampleLibrary.so");
    plugin.load();
    std::cout << "Plugin ID: " << plugin->get_id() << std::endl;
    std::cout << "Plugin processes 5 into: " << plugin->process_data(5) << std::endl;
    return 0;
}

int get_application_version() {
    return 4;
}