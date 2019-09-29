// low level terminal handling classes
// setting terminal in raw mode and reading key presses.

#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>

#ifndef LOW_LEVEL_TERM_H
#define LOW_LEVEL_TERM_H

namespace quark
{   
    // structure to contain window attributes
    struct Window_attr {
        int w_h;    // window height
        int w_w;    // window width
        int w_sy;   // window starting y coordinates
        int w_sx;   // window starting x coordinates 
    };

    // editor modes
    enum class E_mode {INSERT, COMMAND};

    // Following are the editor commands that this editor currently supports
    enum E_command {
        QUIT = 'q',     // quit editor
        WRITE = 'w',    // save everything
        INSERT = 'i',   // put the editor in insert mode and begin editing
        ESC = 27,       // put the editor in command mode and type commands
    };
    
    class Ncurses {
    // class to put the whole screen in ncurses mode
    public:
        Ncurses() {
            initscr(); // start ncurses mode
            refresh();
        }
        ~Ncurses() {
            endwin();
        }
    };

    class Window {
    // class for window attributes and functions
    private:
        // data
        struct Window_attr w_attr;
        WINDOW* win; // pointer to ncurses window
        int cw_y, cw_x; // current y & x coordinates of window
        
        // functions
        WINDOW* create_win(const struct Window_attr & w_attr);
        void destroy_win();

    public:
        Window(const struct Window_attr & w_attr);
        ~Window();
        void write(char c, int y, int x);
        void write(char c);
        void mv_coord(int new_y, int new_x);
    };

    class Ed_text : public Window {
    // class representing text portion of editor
    private:
        std::string buffer; 
    public:
        Ed_text(const struct Window_attr & w_attr);
        ~Ed_text();
    };

    class Co_text : public Window {
    // class representing command section of editor
    private:
        std::string buffer;
    public:
        Co_text(const struct Window_attr & w_attr);
        ~Co_text();
    };

}
#endif
