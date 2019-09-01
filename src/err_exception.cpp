// cpp file implementing common functions
// related to error handling

#include <iostream>
#include <string>
#include <cstring>
#include "err_exception.h"

using std::cerr;
using std::cout;
using std::endl;

void c_exception::die(c_exception::errcode_excep& excep) {
    // print error code, error message, and other info
    // associated with exception excep and exit program

    cerr << "#### Error code: " << excep.e_no <<  " Message: " << excep.e_msg
        << " returned by func: " << excep.e_fname << "####" << endl;

    cerr << "Exiting program. Bye!" << endl;
    exit(-1);
}

void c_exception::die(int e_no, const std::string& fname) {
    // print error code, error message, and other info
    // associated with errno and exit. Overloaded function die

    cerr << "#### Error code: " << e_no <<  " Message: " << std::strerror(e_no)
        << " returned by func: " << fname << "####" << endl;
    
    cerr << "Exiting program. Bye!" << endl;
    exit(-1);
}

