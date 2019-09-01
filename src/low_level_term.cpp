// low level terminal handling classes
// setting terminal in raw mode and reading key presses.

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <cassert>
#include <sys/ioctl.h>
#include "low_level_term.h"
#include "err_exception.h"

using terminal::Term_mode;
using terminal::Editor;
using c_exception::errcode_excep;   // custom exception class

Term_mode::Term_mode() {  // constructor implementation.

    if (tcgetattr(STDIN_FILENO, &orig) == -1) { // get original term attributes.
        c_exception::die(errno, std::string(__func__));
    }   

    raw = orig; // copy into raw term attributes

    term_mode = terminal::t_mode::COOKED; // terminal is still in canonical/cooked mode

    // fill up screen size members
    try {
        fill_window_size();
    }
    catch (errcode_excep& excep) {
        c_exception::die(excep);
    }
}

Term_mode::~Term_mode() { // destructor implementation.
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) { // restore user's original term settings back at exit
        c_exception::die(errno, std::string(__func__));
    }
    
    term_mode = terminal::t_mode::COOKED;
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

    term_mode = terminal::t_mode::RAW; // raw mode set
}

void Term_mode::disable_raw() { // disable raw mode

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) { // restore user's original term settings back
        throw errcode_excep(errno, std::strerror(errno), __func__);
    }

    term_mode = terminal::t_mode::COOKED; // raw mode disabled, canonical mode set

}

void Term_mode::fill_window_size() { 
    // get window size of the terminal
    
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) { // get window size in terminal
        throw errcode_excep(errno, std::strerror(errno), __func__);
    }

    win_col = ws.ws_col;
    win_row = ws.ws_row;

}

Editor::Editor(terminal::e_mode mode /* = terminal::e_mode::COMMAND */) 
    : editor_mode {mode} {
    // default constructor implementation
    
}

Editor::~Editor() { 
    // destructor
    
    write(STDOUT_FILENO, "\x1b[2J", 4); // clear entire screen
    write(STDOUT_FILENO, "\x1b[H", 3);  // reposistion cursor to row 1, column 1
}

void Editor::set_insert_mode() {
    // set the editor in insert mode

    if (editor_mode == terminal::e_mode::INSERT) return; // already in insert mode
    
    try {
        enable_raw();   // put the terminal in raw mode.
    } 
    catch (errcode_excep& excep) {
        c_exception::die(excep);
    }
    
    editor_mode = terminal::e_mode::INSERT; // mark that the editor is in insert mode.

}

void Editor::set_command_mode() {   
    // set the editor in command mode

    if (editor_mode == terminal::e_mode::COMMAND) return;   // already in command mode

    // disable terminal's raw mode
    try { 
        disable_raw();
    }
    catch (errcode_excep& excep) {
        c_exception::die(excep);
    }

    editor_mode = terminal::e_mode::COMMAND; // mark that the editor is in command mode.
}

char Editor::read_key_press() {        
    // function to read key presses entered
    // returns byte read
    
    int by_read = 0;
    char c = '\0';

    while (true) {
        by_read = read(STDIN_FILENO, &c, 1);    // read one byte at a time
        
        if (by_read > 0)    // read some key press entered
            break;
        
        if (by_read == -1 && errno != EAGAIN) 
            throw errcode_excep(errno, std::strerror(errno), __func__);
        else
            continue;
    }

    return c;
}

void Editor::process_key_press() {
    // function to process key presses entered

    char c = read_key_press();

    if (c == 0) return;
    
    // assertion to check the validity of editor mode.
    assert((editor_mode == terminal::e_mode::COMMAND 
            || editor_mode == terminal::e_mode::INSERT) 
            && "Editor mode other than insert or command\n");
    
    if (editor_mode == terminal::e_mode::COMMAND) {     // editor is in command mode
        
        switch(c) {

            case QUIT: exit(0);

            case WRITE: std::cout << "write pressed\n"; break;

            case INSERT: set_insert_mode(); break; 

            default: std::cout << "Unknown command\n"; break;

        }
        
    
    }
    else if (editor_mode == terminal::e_mode::INSERT) { // editor is in insert mode

        if (c == ESC) {  // ESC button pressed, put the editor in command mode
            set_command_mode();
        }
        else {           // some other key pressed
            printf("%d '%c'\r\n", c, c);
        }
    
        
    }

}

void Editor::refresh_screen() {
    // function to refresh screen

    write(STDOUT_FILENO, "\x1b[2J", 4); // clear entire screen
    write(STDOUT_FILENO, "\x1b[H", 3);  // reposistion cursor to row 1, column 1

    draw_rows();    // draw tildes
    
    write(STDOUT_FILENO, "\x1b[H", 3);  // reposistion cursor to row 1, column 1
}

void Editor::draw_rows() {
    // function that draws '~'
  
    for (int y = 0; y < get_screen_rows(); y++) {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}
