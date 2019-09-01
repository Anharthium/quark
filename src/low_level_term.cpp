// low level terminal handling classes
// setting terminal in raw mode and reading key presses.

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <cstring>
#include "low_level_term.h"
#include "err_exception.h"

using terminal::Term_mode;
using c_exception::errcode_excep;   // custom exception class

Term_mode::Term_mode() {  // constructor implementation.

    if (tcgetattr(STDIN_FILENO, &orig) == -1) { // get original term attributes.
        c_exception::die(errno, std::string(__func__));
    }   

    raw = orig; // copy into raw term attributes

    mode = 'c'; // terminal is still in canonical mode
}

Term_mode::~Term_mode() { // destructor implementation.
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) { // restore user's original term settings back at exit
        c_exception::die(errno, std::string(__func__));
    }
    mode = 'c';
}

void Term_mode::enable_raw() { // enable raw mode
    
    // turn off echoing (ECHO bit mask)
    // turn off canonical mode (read input byte by byte) (ICANON bit mask)
    // turn off cntrl-c (ISIG) and cntrl-v (IEXTEN)
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    
    // turn off cntrl-s and cntrl-q (freeze and resume terminal) (IXON)
    // disable carriage return \r. fix for cntrl-m (ICRNL)
    // the following bits probably should be turned off by default.
    // disable break condition (BRIKINT) and parity checking (INPCK)
    // disable stripping of 8th byte (ISTRIP)
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | ICRNL | IXON);

    // turn off all output processing (\n to \r\n) (OPOST bit mask)
    raw.c_oflag &= ~(OPOST);

    // set character size as 8 bits (CS8). CS8 contains multiple bits already set.
    raw.c_cflag |= (CS8);

    // set timeout for read
    raw.c_cc[VMIN]  = 0;    // minimum time. Return as soon as a byte is read.
    raw.c_cc[VTIME] = 1;    // 100 ms max time to wait for input. 



    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) { 
        throw errcode_excep(errno, std::strerror(errno), __func__);
    }

    mode = 'r'; // raw mode set
}

void Term_mode::disable_raw() { // disable raw mode

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) { // restore user's original term settings back
        throw errcode_excep(errno, std::strerror(errno), __func__);
    }

    mode = 'c'; // raw mode disabled, canonical mode set

}

