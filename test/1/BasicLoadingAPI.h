#ifndef CPPLUGINS_BASICLOADINGAPI_H
#define CPPLUGINS_BASICLOADINGAPI_H

class BasicLoading {
public:
    virtual ~BasicLoading()= default;
    virtual int get_state();
private:
    int state_ = 1;
};

#endif //CPPLUGINS_BASICLOADINGAPI_H
