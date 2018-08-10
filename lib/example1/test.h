#include "sample_API.hpp"

int get_application_version();

class sample_implementation : public sample_API {
public:
    sample_implementation() {
        _ID = "Sample_Plugin_1" + ((check_version()) ? std::string("_UpToDate"): std::string("_Outdated"));
    }
    std::string get_id() override;
    int process_data(int) override;
private:
    bool check_version();
};

extern "C" {
    sample_API * create();
    void destroy(sample_API *);
}