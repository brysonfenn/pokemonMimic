#include "print_battle.h"

#include <ncurses.h>

#include "print_defines.h"

#include "../player.h"
#include "../monsters/pokemon.h"

#define BLINK_TIME_MICROS 100000

void adjust_cursors(int selection, int start_x, int start_y);

static int cursor_x;
static int cursor_y;

//Print Battle Box and basic pokemon information
void printBattle() {
  Pokemon * player_pok = player.current_pokemon;
  Pokemon * enemy_pok = player.enemy_pokemon;
  char poke_string[128];

  //Draw battle box
  draw_box(BATTLE_BOX_X, BATTLE_BOX_Y, BATTLE_BOX_WIDTH, BATTLE_BOX_HEIGHT);

  sprintf(poke_string, "%s  Lvl %d ", enemy_pok->name, enemy_pok->level);
  add_condition_string(poke_string, enemy_pok);
  mvprintw(BATTLE_BOX_ENEMY_Y, BATTLE_BOX_ENEMY_X, poke_string);
  mvprintw(BATTLE_BOX_ENEMY_Y+1, BATTLE_BOX_ENEMY_X, "HP: % 3d/%d", enemy_pok->currentHP, enemy_pok->maxHP);

  sprintf(poke_string,  "%s  Lvl %d  ", player_pok->name, player_pok->level);
  add_condition_string(poke_string, player_pok);
  mvprintw(BATTLE_BOX_PLAYER_Y, BATTLE_BOX_PLAYER_X, poke_string);
  mvprintw(BATTLE_BOX_PLAYER_Y+1, BATTLE_BOX_PLAYER_X, "HP: % 3d/%d", player_pok->currentHP, player_pok->maxHP);

  print_btn_instructions(BATTLE_BOX_X+BATTLE_BOX_WIDTH+1, TEXT_BOX_Y, false);

  //Draw text box
  draw_box(TEXT_BOX_X, TEXT_BOX_Y, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);
  refresh();
}


//Blink enemy if enemy is true, if not, blink player
void blinkPokemon(bool blink_player, int color, int num_times) {
  Pokemon * pok;
  char poke_string[128];
  int text_x, text_y;

  //Set up location and pokemon depending on whether enemy attacked
  if (!blink_player) {
    pok = player.enemy_pokemon;
    text_x = BATTLE_BOX_ENEMY_X;
    text_y = BATTLE_BOX_ENEMY_Y;
    
  }
  else {
    pok = player.current_pokemon;
    text_x = BATTLE_BOX_PLAYER_X;
    text_y = BATTLE_BOX_PLAYER_Y;
  }

  //Prepare string
  sprintf(poke_string, "%s  Lvl %d ", pok->name, pok->level);
  add_condition_string(poke_string, pok);

  attrset(COLOR_PAIR(color));
  //Blink pokemon
  for (int i = 0; i < num_times; i++) {
    mvprintw(text_y, text_x, "                       ");
    mvprintw(text_y+1, text_x, "                       ");
    refresh(); usleep(BLINK_TIME_MICROS);
    mvprintw(text_y, text_x, poke_string);
    mvprintw(text_y+1, text_x, "HP: % 3d/%d", pok->currentHP, pok->maxHP);
    refresh(); usleep(BLINK_TIME_MICROS);
  }
  attrset(COLOR_PAIR(DEFAULT_COLOR));
  mvprintw(text_y, text_x, poke_string);
  mvprintw(text_y+1, text_x, "HP: % 3d/%d", pok->currentHP, pok->maxHP);
  refresh();
}


//Get selection from the user for battle: FIGHT, BAG, POKEMON, or RUN
int get_battle_selection(int first_line, int last_selection) {
  int selection = last_selection;
  cursor_x = BATTLE_SELECT_1_X;
  cursor_y = first_line;

  adjust_cursors(selection, BATTLE_SELECT_1_X, first_line);

  int ch;

  flushinp();
  while (1) {
    ch = getch();

    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
      case KEY_DOWN:
        if (selection == 0) selection = 2;
        else if (selection == 1) selection = 3;
        else if (selection == 2) selection = 0;
        else if (selection == 3) selection = 1;
        break;
      case KEY_LEFT:
        if (selection == 0) selection = 3;
         else selection--;
        break;
      case KEY_RIGHT:
         if (selection == 3) selection = 0;
         else selection++;
        break;
      case 'a':
        return (selection);
      default:
        break;
    }
    adjust_cursors(selection, BATTLE_SELECT_1_X, first_line);
  }
}


//Get selection from the user for a pokemon move to use
int get_move_selection(int start_x, int start_y, struct Pokemon* pok) {
  int selection = 0;
  cursor_x = start_x;
  cursor_y = start_y;
  attack currAttack;

  int ch;

  flushinp();
  while (1) {
    mvprintw(start_y+3, start_x+2, "\t\t\t\t\t");
    if (selection != 4) {
      currAttack = pok->attacks[selection];
      mvprintw(start_y+3, start_x+2, "%s", get_type_string_by_id(currAttack.type));
      mvprintw(start_y+3, start_x+14, "PP: %d/%d", currAttack.curr_pp, currAttack.max_pp);
    }

    adjust_cursors(selection, start_x, start_y);

    ch = getch();
    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
      case KEY_DOWN:
        if (selection == 0 && player.current_pokemon->numAttacks > 2) 
          selection = 2;
        else if (selection == 1 && player.current_pokemon->numAttacks > 3) 
          selection = 3;
        else if (selection == 2) selection = 0;
        else if (selection == 3) selection = 1;
        break;
      case KEY_LEFT:
        if (selection == 0) selection = 4;
        else if (selection == 4) selection = pok->numAttacks - 1;
        else selection--;
        break;
      case KEY_RIGHT:
         if (selection == pok->numAttacks - 1 || selection == 3) selection = 4;
         else if (selection == 4) selection = 0;
         else selection++;
        break;
      case 'a':
        //Do not allow if PP is gone
        if (pok->attacks[selection].curr_pp == 0 && selection != 4) {
          text_box_cursors(TEXT_BOX_BEGINNING);
          printw("There is no PP left for %s", pok->attacks[selection].name); refresh(); sleep(2);
          clear_text_box();
          continue;
        }

        clear_selection_text();
        return (selection);
      case 'b':
        return (PRESSED_B);
      default:
        break;
    }
  }
}

void adjust_cursors(int selection, int start_x, int start_y) {
  switch (selection) {
    case 0:
      cursor_x = start_x; cursor_y = start_y;
      break;
    case 1:
      cursor_x = start_x+MOVE_SELECT_SPACING; cursor_y = start_y;
      break;
    case 2:
      cursor_x = start_x; cursor_y = start_y+1;
      break;
    case 3:
      cursor_x = start_x+MOVE_SELECT_SPACING; cursor_y = start_y+1;
      break;
    case 4:
      cursor_x = start_x+MOVE_SELECT_SPACING*2; cursor_y = start_y+1;
      break;
    default:
      cursor_x = start_x+MOVE_SELECT_SPACING*2; cursor_y = start_y;
      break;
  }

  mvaddch(cursor_y, cursor_x, SELECTION_CHAR);
  refresh();
}


static int text_box_cursor_y = 0;

//Move the cursor to print to the text box
//  if next_line == TEXT_BOX_BEGINNING, clear and restart text box
//  if next_line == TEXT_BOX_NEXT_LINE, move to the next line, or restart if on last line
void text_box_cursors(int next_line) {
  if (text_box_cursor_y == 3) next_line = 0;  //Only print 4 lines in text box
  //Clear and reseet if next_line is 0
  if (!next_line) {
    clear_text_box();
    text_box_cursor_y = 0;
  }           
  else text_box_cursor_y++;                   //Increment if next_line is 1
  move(TEXT_BOX_Y+5 + text_box_cursor_y, TEXT_BOX_X+3);
}

//Clear all text from the battle box
void clear_battle_box() {
  for (int i = 0; i < (BATTLE_BOX_HEIGHT - 2); i++) {
    mvprintw(BATTLE_BOX_Y+1+i, BATTLE_BOX_X+1, "                                               ");                           
  }
  refresh();
}

//Clear all text from the text box where selections are made
void clear_selection_text() {
  for (int i = 0; i < 4; i++) {
    mvprintw(TEXT_BOX_Y+1+i, TEXT_BOX_X+1, "                                               ");                           
  }
}

//Clear all text from the text box where messages are displayed
void clear_text_box() {
  for (int i = 0; i < 4; i++) {
    mvprintw(TEXT_BOX_Y+5+i, TEXT_BOX_X+1, "                                               ");                           
  }
}