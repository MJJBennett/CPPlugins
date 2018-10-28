#include "BasicLoadingLib.h"
#include <iostream>

int BasicLoadingLib::get_state() { return state_; }

void destroy(BasicLoading *obj) {
    delete obj;
}

BasicLoading *create(int state) {
    return new BasicLoadingLib(state);
}
