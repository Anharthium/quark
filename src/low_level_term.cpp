// low level terminal handling classes
// setting terminal in raw mode and reading key presses.

#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <cassert>
#include "low_level_term.h"
#include "err_exception.h"

namespace quark {

Window::Window(const struct Window_attr & w_attr) {
    // construct window with specified attributes
    
    raw();     // disable line buffering; enable raw mode
    noecho();  // don't echo
     
    win = create_win(w_attr);
    keypad(win, TRUE); // enable arrow keys and F1, F2 etc.
    cw_y = w_attr.w_sy;
    cw_x = w_attr.w_sx;
}

Window::~Window() {
    // destruct window; deallocate resources
    
    destroy_win(); // delete current window
    endwin(); // end curses mode
}

WINDOW* Window::create_win(const struct Window_attr & w_attr) {
    // function to create window with specified attributes 
    // and return pointer to structure associated with that 
    // window
    WINDOW* curr_win = newwin(w_attr.w_h, w_attr.w_w, w_attr.w_sy, w_attr.w_sx);
    //box(curr_win, 0, 0); // border with default char for vert and hor lines
    wrefresh(curr_win);  // show that window
    return curr_win;
}

void Window::destroy_win() {
    // function to delete the current window and free any 
    // resources acquired

    // The parameters taken are 
	// 1. win: the window on which to operate
	// 2. ls: character to be used for the left side of the window 
	// 3. rs: character to be used for the right side of the window 
	// 4. ts: character to be used for the top side of the window 
	// 5. bs: character to be used for the bottom side of the window 
	// 6. tl: character to be used for the top left corner of the window 
	// 7. tr: character to be used for the top right corner of the window 
	// 8. bl: character to be used for the bottom left corner of the window 
	// 9. br: character to be used for the bottom right corner of the window
    wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' '); // draw border with the following chars
    wrefresh(win);
    delwin(win);
}

void Window::write(char c, int y, int x) {
    // write a char to to coordinates y and x; 
    // advance coordinates y and x
    mvwaddch(win, y, x, c);
    wrefresh(win);
}

void Window::write(char c) {
    // write a char to to current coordinates; 
    // advance coordinates y and x
    waddch(win, c);
    wrefresh(win);
}

void Window::mv_coord(int new_y, int new_x) {
    // move current window coordinates to new_y and new_x
}

Ed_text::Ed_text(const struct Window_attr & w_attr): Window(w_attr) {
    buffer = '\0';
}

Ed_text::~Ed_text() {
}

Co_text::Co_text(const struct Window_attr & w_attr): Window(w_attr) {
    buffer = '\0';
}

Co_text::~Co_text() {

}

} 
