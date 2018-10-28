#ifndef CPPLUGINS_BASICLOADINGLIB_H
#define CPPLUGINS_BASICLOADINGLIB_H

#include "BasicLoadingAPI.h"

class BasicLoadingLib {
public:
    BasicLoadingLib(int state) : state_(state);
    int get_state() override;
};


#endif //CPPLUGINS_BASICLOADINGLIB_H
