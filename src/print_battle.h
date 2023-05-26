#ifndef PRINT_BATTLE_H
#define PRINT_BATTLE_H

#include <stdbool.h>

struct Pokemon;

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

#endif //PRINT_BATTLE_H