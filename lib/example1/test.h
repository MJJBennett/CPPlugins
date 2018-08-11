#ifndef CPPLUGINS_TEST1_H
#define CPPLUGINS_TEST1_H

#include "sample_API.hpp"

class sample_implementation : public sample_API {
public:
    sample_implementation()=default;
    std::string get_id() override;
    int process_data(int) override;
    void init() override;
private:
    bool check_version();
};

extern "C" {
    sample_API * create();
    void destroy(sample_API *);
}

#endif //CPPLUGINS_TEST1_H