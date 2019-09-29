// wrapper class to wrap C library
// function's error codes
// to cpp exceptions.

#include <string>
#include <exception>

#ifndef C_EXCEPTION_H
#define C_EXCEPTION_H

namespace c_exception
{
    // custom exception class
    class errcode_excep: public std::exception {
    
    public:
        int e_no;          // error code
        std::string e_msg; // error message
        std::string e_fname; // function which returned error code
        
        errcode_excep(int code, const std::string& msg, const std::string& fname)   // custom exception constructor 
            : e_no {code}, e_msg {msg}, e_fname {fname}
        { }
        
        errcode_excep(int code, std::string&& msg, std::string&& fname)   // move constructor
            : e_no {code}, e_msg {msg}, e_fname {fname}
        { }
    };

    void die(errcode_excep& excep); // function to kill process after displaying 
                                    // information contained in exception.
                                    
    void die(int e_no, const std::string& fname);   // overloaded function die for displaying information 
                                                    // about error without using errcode_excep class.

}

#endif
