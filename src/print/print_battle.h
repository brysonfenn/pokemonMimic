#ifndef PRINT_BATTLE_H
#define PRINT_BATTLE_H

#include <stdbool.h>

struct Pokemon;

#define DAMAGE_BLINK_TIMES 3
#define LEECH_SEED_BLINK_TIMES 2

#define TEXT_BOX_BEGINNING 0
#define TEXT_BOX_NEXT_LINE 1

//Print Battle Box and basic pokemon information
void printBattle();

//Blink enemy if enemy is true, if not, blink player
void blinkPokemon(bool blink_player, int color, int num_times);

//Get selection from the user for battle: FIGHT, BAG, POKEMON, or RUN
int get_battle_selection(int first_line, int last_selection);

//Get selection from the user for a pokemon move to use
int get_move_selection(int start_x, int start_y, struct Pokemon* pok);

//Move the cursor to print to the text box
//  if next_line == TEXT_BOX_BEGINNING, clear and restart text box
//  if next_line == TEXT_BOX_NEXT_LINE, move to the next line, or restart if on last line
void text_box_cursors(int next_line);

//Clear all text from the battle box
void clear_battle_box();

//Clear all text from the text box where selections are made
void clear_selection_text();

//Clear all text from the text box where messages are displayed
void clear_text_box();


#endif //PRINT_BATTLE_H