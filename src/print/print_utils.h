#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <stdbool.h>

#define TEXT_BOX_BEGINNING 0
#define TEXT_BOX_NEXT_LINE 1

#define PRESSED_B -1

struct Pokemon;

//print_utils.c

//Print the player's current pokemon party
void printParty();

//Print the player's current bag
void printBag();

//Initialize ncurses library - including colors, extended character library, etc.
void resume_ncurses();

//End ncurses library functionality
void pause_ncurses();

//Get player selection from list
//  first_line is the first line of the list (0 is the top of the terminal)
//  num_options is the number of options in the list available to select
//  last_selection is where the cursor will begin (usually 0 or the last option selected)
int get_selection(int first_line, int num_options, int last_selection);

//Draw simple box with top-left corner (x,y) and with width and height (w,h)
void draw_box(int x, int y, int w, int h);

//Print the instruction box at a given location (x,y)
//  If on_map is true, the special instruction to go to menu will be shown
void print_btn_instructions(int x, int y, bool on_map);

//Erase current list and resetart at the beginning
void begin_list();

//Print to the next line of the list
// list_str must be a string constant, and end_line characters are understood
// to indicate a next list item
void print_to_list(const char * list_str);

//Save print state to print later
void save_print_state();

//Reprint the saved print state - cannot be called without first calling save_print_state
void restore_print_state();

#endif // PRINT_UTILS_H