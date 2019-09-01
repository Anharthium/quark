// low level terminal handling classes
// setting terminal in raw mode and reading key presses.

#include <iostream>
#include <termios.h>
#include <unistd.h>

#ifndef LOW_LEVEL_TERM_H
#define LOW_LEVEL_TERM_H

namespace terminal
{

    // class to set terminal in raw mode and 
    // back to cannonical mode when the program exits 
    class Term_mode  
    {
    private:
        struct termios orig; // original terminal attributes, 
                             // to be restored back at exit
        struct termios raw;  // custom terminal attributes to be set 
                             // for enabling raw mode
        char mode;           // terminal mode: 'r' for raw, 'c' for cannonical
    
    public:
        Term_mode();         // default constructor. Just copies original 
                            // terminal attributes to raw structure

        ~Term_mode();        // destructor. Sets terminal attributes back at exit.

        void enable_raw();  // for enabling raw mode
        void disable_raw(); // for disabling raw mode

        inline char get_mt() { return mode; }      // get mode type, 'r' or 'c'
                 
    };


}
#endif
