#ifndef CPPLUGINS_BASICLOADINGLIB_H
#define CPPLUGINS_BASICLOADINGLIB_H

#include "BasicLoadingAPI.h"

class BasicLoadingLib : public BasicLoading {
public:
    explicit BasicLoadingLib(int state) : BasicLoading(state) {}
    int get_state() override;
};


extern "C" {
BasicLoading *create(int state);
void destroy(BasicLoading * obj);
}


#endif //CPPLUGINS_BASICLOADINGLIB_H
