#include <iostream>
#include "cpplugins.hpp"
#include "BasicLoadingAPI.h"

int main() {
    using namespace cpl;
    Plugin<BasicLoading> plugin(dl_path("libTest1Lib"), Flag::None);
    if (!plugin.is_init()) return static_cast<int>(plugin.get_state());
    return 0; // Success
}