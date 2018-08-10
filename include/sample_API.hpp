#ifndef CPPLUGINS_SAMPLE_INTERFACE_H
#define CPPLUGINS_SAMPLE_INTERFACE_H

#include <string>

/**
 * This is a sample API the application designer could provide for plugins.
 */
class sample_API {
public:
    //An example method that arbitrarily processes an int then returns the result.
    virtual int process_data(int)=0;

    //An example method that retrieves a private field.
    virtual inline std::string get_id() {
        return _ID;
    }

    //A virtual destructor is (apparently) necessary to ensure any subclasses are properly deleted.
    virtual ~sample_API()=default;

protected:
    //An example protected field.
    std::string _ID = "UNIDENTIFIED";
};

#endif //CPPLUGINS_SAMPLE_INTERFACE_H
