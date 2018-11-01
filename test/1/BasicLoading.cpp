#include <iostream>
#include "cpplugins.hpp"
#include "BasicLoadingAPI.h"

int main() {
    using namespace cpl;
    Plugin<BasicLoading, int> plugin(dl_path("./libTest1Lib"), Flag::None, 5);
    if (!plugin.is_init()) return plugin.get_state<int>();
    std::cout << plugin->get_state() << std::endl;
    return 0; // Success
}