#include "sample_interface.hpp"

void do_something_else();

class sample_implementation : public sample_interface {
public:
    void do_something() override;
    int get_something() override;
private:
    void private_do_something();
};

extern "C" {
    sample_interface *  get();
    void destroy(sample_interface *);
}