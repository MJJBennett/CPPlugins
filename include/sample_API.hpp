#ifndef CPPLUGINS_SAMPLE_INTERFACE_H
#define CPPLUGINS_SAMPLE_INTERFACE_H

class sample_API {
public:
    virtual void do_something()=0;
    virtual int get_something()=0;
    virtual ~sample_API()=default;
};

#endif //CPPLUGINS_SAMPLE_INTERFACE_H
