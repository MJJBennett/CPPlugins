#include <iostream>
#include "sample_API.hpp"
#include "cpplugins.hpp"

int get_application_version() {
    return 4;
}

int main() {
    std::string plugin_path = "./lib/example1/libExampleLibrary";
    cpl::Plugin<sample_API> plugin(cpl::dl_path(plugin_path), std::nullopt);
    plugin.load();
    plugin->set("get_application_version", cpl::make_void(get_application_version));
    plugin->init();
    std::cout << "Plugin ID: " << plugin->get_id() << std::endl;
    std::cout << "Plugin processes 5 into: " << plugin->process_data(5) << std::endl;
    return 0;
}

