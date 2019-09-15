// low level terminal handling classes
// setting terminal in raw mode and reading key presses.

#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>

#ifndef LOW_LEVEL_TERM_H
#define LOW_LEVEL_TERM_H

namespace terminal
{   
    // structure to contain window attributes
    struct Window_attr {
        std::string w_name; // name
        int w_h;    // window height
        int w_w;    // window width
        int w_sy;   // window starting y coordinates
        int w_sx;   // window starting x coordinates 
    };

    // editor modes
    enum class e_mode {INSERT, COMMAND};

    // Following are the editor commands that this editor currently supports
    enum e_command {
        QUIT = 'q',     // quit editor
        WRITE = 'w',    // save everything
        INSERT = 'i',   // put the editor in insert mode and begin editing
        ESC = 27,       // put the editor in command mode and type commands
    };


}
#endif
