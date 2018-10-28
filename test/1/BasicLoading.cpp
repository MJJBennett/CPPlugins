#include <iostream>
#include "cpplugins.hpp"
#include "BasicLoadingAPI.h"

int main() {
    using namespace cpl;
    Plugin<BasicLoading> plugin(dl_path("test/1/Test1Lib"));
    if (!plugin.is_init())
    return 0;
}