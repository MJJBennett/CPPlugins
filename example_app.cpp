#include <iostream>
#include "sample_API.hpp"
#include "cpplugins.hpp"

int get_application_version() {
    return 4;
}

void run_tests() {
    std::cout << "Starting testing." << std::endl;
    std::string test_path = "./lib/test1/libTestLibrary1";
    cpl::State plugin_state{};
    cpl::Plugin<sample_API> test_plugin(cpl::dl_path(test_path), plugin_state);
    if (!test_plugin.is_init()) std::cout << "Could not initialize the library." << std::endl;
    std::cout << static_cast<int>(plugin_state) << std::endl;
    test_plugin->set("get_application_version", cpl::make_void(get_application_version));
    test_plugin->init();
    if (test_plugin->get_id() != "UNIDENTIFIED") std::cout << "Failed test." << std::endl;
    if (test_plugin->add(4, 5) != 9) std::cout << "Failed test." << std::endl;
    std::cout << "Finished testing." << std::endl;
}

int main() {
//    std::string plugin_path = "./lib/example1/libExampleLibrary";
//    cpl::Plugin<sample_API> plugin(cpl::dl_path(plugin_path), std::nullopt);
//    plugin->set("get_application_version", cpl::make_void(get_application_version));
//    plugin->init();
//    std::cout << "Plugin ID: " << plugin->get_id() << std::endl;
//    std::cout << "Plugin processes 5 into: " << plugin->process_data(5) << std::endl;

    run_tests();

    return 0;
}

