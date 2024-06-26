#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <stdbool.h>

struct Pokemon;

#define DEFAULT_NAME_STR "~`~"
#define CANCEL_STR "`~`"


//Print a list of Pokemon (used for party and PC)
void print_pokemon_list(struct Pokemon * pokList, int list_size);

//Print a list of Pokemon (used for party and PC)
int get_pc_selection(int last_selection);

//Print the player's current pokemon party
void printParty();

//Print the player's party, with able/unable based on size [6] bool array
void print_party_able_unable(const bool able_array[6]);

//Print the player's current bag
void printBag();

//Initialize ncurses library - including colors, extended character library, etc.
void resume_ncurses();

//End ncurses library functionality
void pause_ncurses();

//Get player selection from list
//  first_line is the first line of the list (0 is the first line of the list_box)
//  highest_option_num is the number of options in the list available to select
//  last_selection is where the cursor will begin (usually 0 or the last option selected)
int get_selection(int first_line, int highest_option_num, int last_selection);

//Draw simple box with top-left corner (x,y) and with width and height (w,h)
void draw_box(int x, int y, int w, int h);

//Print the instruction box at a given location (x,y)
//  If on_map is true, the special instruction to go to menu will be shown
void print_btn_instructions(bool on_map);

//Wait for user to press the selection button ('z')
int await_user();

//Erase current list and resetart at the beginning
void begin_list();

//Insert end lines to avoid text box overflow
void fix_list_box_overflow(char * input);

//Print to the next line of the list
// list_str must be a string constant, and end_line characters are understood
// to indicate a next list item
void print_to_list(const char * list_str);

//Save print state to print later
void save_print_state();

//Reprint the saved print state - cannot be called without first calling save_print_state
void restore_print_state();

//Get name input from user -- Returns DEFAULT_NAME_STR if player enters without typing
char * get_name_input(char * target_for_name);

//Get player selection from scrollable list
//  str_list is the list of strings representing elements in list
//  num_in_list is the number of strings in the list
//  last_selection is where the cursor will begin (usually 0 or the last option selected)
int get_scrollable_selection(char * title, char ** str_list, int num_in_list, int last_selection);

#endif // PRINT_UTILS_H