// quark -- quark text editor.
// Small, yet functionable and efficient text editor for everyday use.
//
// 
// 

#include <iostream>
#include <string>
#include <ncurses.h>
#include <cstring>
#include <ctype.h>
#include <errno.h>
#include "low_level_term.h"
#include "err_exception.h"

using c_exception::errcode_excep; // custom exception
using std::cout;
using std::cerr;
using std::endl;
using quark::E_mode;
using quark::Window_attr;
using quark::Ed_text;
using quark::Co_text;

// ------------------------------------------- function prototypes --------------------------------------
int process_input();

// ------------------------------------------- global variables --------------------------------------

// start ncurses mode
quark::Ncurses ncurses;

// start editor in editor mode
E_mode editor_mode = E_mode::INSERT;

struct Window_attr w_attr_t = { // for text portion
    .w_h = LINES - 2,
    .w_w = COLS,
    .w_sy = 0,
    .w_sx = 0 
};

Ed_text t_editor(w_attr_t); // to handle text portions

struct Window_attr w_attr_c = { // for command portion
    .w_h = 2,
    .w_w = COLS,
    .w_sy = LINES - 2,
    .w_sx = 0
};
    
Ed_text c_editor(w_attr_c); // to handle command portions

// ------------------------------------------- global variables --------------------------------------

int main() {

    // continuosly process input
    while (1) {
        if (process_input() == -1) break;
    }

}

int process_input() {
    // function to process keypresses from user
    // return -1 to quit, else characters entered

    char c = getch();

    if (editor_mode == E_mode::INSERT) { // editor in insert mode
        if (c == quark::ESC) editor_mode = E_mode::COMMAND;
        else {
            t_editor.write(c);
        }    
    }
    else { // editor in command mode
        switch (c) {
            case quark::QUIT: c = -1; 
                              break;
            case quark::INSERT: editor_mode = E_mode::INSERT; // switch to insert mode
                                break; 
            default: break;
        }
    }
    
    return c;
}
