#ifndef CPPLUGINS_BASICLOADINGAPI_H
#define CPPLUGINS_BASICLOADINGAPI_H

class BasicLoading {
public:
    explicit BasicLoading(int state) : state_(state) {}
    virtual ~BasicLoading()= default;
    virtual int get_state() = 0;
protected:
    int state_ = 1;
};

#endif //CPPLUGINS_BASICLOADINGAPI_H
