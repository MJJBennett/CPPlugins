#include "sample_API.hpp"

void do_something_else();

class sample_implementation : public sample_API {
public:
    void do_something() override;
    int get_something() override;
private:
    void private_do_something();
};

extern "C" {
    sample_API *  get();
    void destroy(sample_API *);
}