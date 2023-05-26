#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <stdbool.h>

#define TEXT_BOX_BEGINNING 0
#define TEXT_BOX_NEXT_LINE 1

#define PRESSED_B -1

struct Pokemon;

//print_utils.c

void printParty();
void printBag();

void resume_ncurses();
void pause_ncurses();

int get_selection(int first_line, int start, int end, int last_selection);

void draw_box(int x, int y, int w, int h);

void print_btn_instructions(int x, int y, bool on_map);

void begin_list();
void print_to_list(const char * list_str);

void save_print_state();
void restore_print_state();

#endif // PRINT_UTILS_H