// quark -- quark text editor.
// Small, yet functionable and efficient text editor for everyday use.
//
// 
// 




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

using terminal::Editor;

// global since its destructor 
// should be called after the program exits
Editor editor{};

int main() {

    while (1) {
        editor.refresh_screen();
        editor.process_key_press();
    }
    
    return 0;
}
