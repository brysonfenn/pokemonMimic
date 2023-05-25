#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <stdbool.h>

#define INVALID_INPUT -9

#define TEXT_BOX_BEGINNING 0
#define TEXT_BOX_NEXT_LINE 1

#define PRESSED_B -1

struct Pokemon;

//print_utils.c

void clearTerminal();
void clearLastLine();

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines);

int getValidInput(int beginRange, int endRange, const char * request);

void printParty();
void printBag();

void resume_ncurses();
void pause_ncurses();

int get_selection(int first_line, int start, int end, int last_selection);

void draw_box(int x, int y, int w, int h);

void print_btn_instructions(int x, int y, bool on_map);

void begin_list();
void print_to_list(const char * list_str);


//print_utils_battle.c

//Print Battle Box and basic pokemon information
void printBattle();

//Blink enemy if enemy is true, if not, blink player
void blinkPokemon(bool enemy);

int get_battle_selection(int first_line, int last_selection);
int get_move_selection(int start_x, int start_y, struct Pokemon* pok);

void draw_text_box();
void text_box_cursors(int next_line);
void clear_battle_box();
void clear_selection_text();
void clear_text_box();

void draw_battle_box();

#endif // PRINT_UTILS_H