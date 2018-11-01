#include <iostream>
#include "sample_API.hpp"
#include "cpplugins.hpp"
#include "test/testing.hpp"

int get_application_version() {
    return 4;
}

int test_lib_1() {
    const std::string test_path = "./lib/test1/libTestLibrary1";
    int fails = 0;

    // Create the plugin wrapper & load the library
    cpl::Plugin<sample_API> test_plugin(cpl::dl_path(test_path), cpl::Flag::None);

    // Test that the plugin was initialized properly
    hard_test_eq(test_plugin.is_init(), true, "PluginIsInitialized");
    fails += test_eq(test_plugin.get_state<int>(), 0, "PluginStateIsZero");

    // Set up the test plugin
    test_plugin->set("get_application_version", cpl::make_void(get_application_version));
    test_plugin->init();

    // Test misc functions
    fails += test_eq(test_plugin->get_id(), std::string("Sample_Plugin_1_Outdated"), "PluginGetID");
    fails += test_eq(test_plugin->add(4, 5), 9, "PluginAdd");

    return fails;
}

int test_lib_example() {
    std::string plugin_path = "./lib/example1/libExampleLibrary";
    int fails = 0;

    // Create the plugin wrapper & load the library
    cpl::Plugin<sample_API> plugin(cpl::dl_path(plugin_path), cpl::Flag::None);

    // Test that the plugin was initialized properly
    hard_test_eq(plugin.is_init(), true, "PluginIsInitialized");
    fails += test_eq(plugin.get_state<int>(), 0, "PluginStateIsZero");

    // Set up the test plugin
    plugin->set("get_application_version", cpl::make_void(get_application_version));
    plugin->init();

    fails += test_eq(plugin->get_id(), std::string("Sample_Plugin_1_Outdated"), "PluginGetID");
    fails += test_eq(plugin->process_data(5), 10, "PluginProcessData");

    return fails;
}

int main() {

    int fails = 0;

    fails += test_lib_1();
    fails += test_lib_example();

    return fails;
}

