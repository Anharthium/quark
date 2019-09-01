/* quark -- quark text editor.
 * Small, yet functionable and efficient text editor for everyday use.
 *
 * 
 * */




#include <iostream>
#include <string>
#include <cstring>
#include <ctype.h>
#include <errno.h>
#include "low_level_term.h"
#include "err_exception.h"

using c_exception::errcode_excep; // custom exception
using std::cout;
using std::cerr;
using std::endl;

using terminal::Term_mode;

// global since its destructor 
// should be called after the program exits
// we don't catch the excepion now
Term_mode term_attr;

int main() {

    
    try {
        term_attr.enable_raw(); // put terminal in raw mode
    }
    catch (errcode_excep& excep) {
        c_exception::die(excep);
    }

    while (1) {
        
        char c = 0;
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
            c_exception::die(errno, std::string(__func__));

        if (c == 'q') break;
        
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } 
        else {
            printf("%d ('%c')\r\n", c, c);
        }
    }
  

    
    return 0;
}
