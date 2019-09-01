// low level terminal handling classes
// setting terminal in raw mode and reading key presses.

#include <iostream>
#include <termios.h>
#include <unistd.h>

#ifndef LOW_LEVEL_TERM_H
#define LOW_LEVEL_TERM_H

namespace terminal
{
    // editor modes
    enum class e_mode {INSERT, COMMAND};
    // terminal modes
    enum class t_mode {RAW, COOKED};

    // Following are the editor commands that this editor currently supports
    enum e_command {
        QUIT = 'q',     // quit editor
        WRITE = 'w',    // save everything
        INSERT = 'i',   // put the editor in insert mode and begin editing
        ESC = 27,       // put the editor in command mode and type commands
    };


    // abstract class to set terminal in raw mode and 
    // back to cannonical mode when the program exits 
    class Term_mode  
    {
    private:
        struct termios orig; // original terminal attributes, 
                             // to be restored back at exit
        struct termios raw;  // custom terminal attributes to be set 
                             // for enabling raw mode
        t_mode term_mode;    // which mode the terminal is currently in? 
                             // raw or cannonical (aka cooked)
        int win_row, win_col;// row and column size of terminal/screen
    
    public:
        Term_mode();         // default constructor. Just copies original 
                             // terminal attributes to raw structure

        virtual ~Term_mode() = 0; // pure virtual destructor. 
                                  // Sets terminal attributes back at exit.
                                  // pure virtual destructor to make this class abstract

        void enable_raw();  // for enabling raw mode
        void disable_raw(); // for disabling raw mode

        inline t_mode get_term_mode() { return term_mode; }      // get terminal mode type

        inline int get_screen_rows() { return win_row; }        // get screen rows
        
        inline int get_screen_cols() { return win_col; }        // get screen cols

        void fill_window_size(); // fills window size of terminal
                 
    };

    class Editor: public Term_mode { 
    // class to handle editor related configuration
    // and other information. Stores and controls whether the editor is in 
    // command mode or insert mode.
    
    private:
        e_mode editor_mode;   // editor mode: INSERT for insert mode, COMMAND for command mode.
    
    public:
        Editor(e_mode = e_mode::COMMAND); // constructor. By default, it opens in command mode.
        ~Editor();               // destructor.

        void set_insert_mode();  // puts the editor in insert mode, 
                                 // where users can insert/edit text
        void set_command_mode(); // puts the editor in command mode,
                                 // where users can't insert text but enter commands
                                 // to modify text, i.e save, exit etc.
        inline e_mode get_editor_mode() { // get the current editor mode
            return editor_mode;
        }

        char read_key_press();   // read key presses and return it.   
        void process_key_press();// process key presses and take appropriate action.
        void refresh_screen();   // refresh screen
        void draw_rows();        // draw tildes
    };
}
#endif
