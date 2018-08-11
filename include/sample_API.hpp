#ifndef CPPLUGINS_SAMPLE_INTERFACE_H
#define CPPLUGINS_SAMPLE_INTERFACE_H

#include <string>
#include <map>

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

    //Used to 'give' the plugin library functions from the main
    virtual void set(std::string name, void * function) {
        _symbols[name] = function;
    }

    //Function that will be called once the symbol table is filled. 
    //TODO - Try using a map pointer to a 'global' map.
    virtual void init()=0;

protected:
    //An example protected field.
    std::string _ID = "UNIDENTIFIED";

    //A possible map of symbols
    std::map<std::string, void *> _symbols;
};

#endif //CPPLUGINS_SAMPLE_INTERFACE_H
