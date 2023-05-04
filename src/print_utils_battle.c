#include "print_utils.h"

#include <ncurses.h>

#include "print_defines.h"
#include "player.h"

#include "monsters/pokemon.h"

void adjust_cursors(int selection, int first_line);

static int cursor_x;
static int cursor_y;

void printBattle() {
  pokemon * player_pok = player.current_pokemon;
  pokemon * enemy_pok = player.enemy_pokemon;
  draw_battle_box();
  mvprintw(BATTLE_BOX_Y+1, BATTLE_BOX_X+3, "\t\t\t%s  Lvl %d  ", enemy_pok->name, enemy_pok->level);
  print_condition(enemy_pok);
  mvprintw(BATTLE_BOX_Y+2, BATTLE_BOX_X+3, "\t\t\tHP: %d/%d", enemy_pok->currentHP, enemy_pok->maxHP);
  mvprintw(BATTLE_BOX_Y+4, BATTLE_BOX_X+3, "%s  Lvl %d  ", player_pok->name, player_pok->level);
  print_condition(player_pok);
  mvprintw(BATTLE_BOX_Y+5, BATTLE_BOX_X+3, "HP: %d/%d", player_pok->currentHP, player_pok->maxHP);

  draw_text_box();
  refresh();
}

int get_battle_selection(int first_line, int last_selection) {
  int selection = last_selection;
  cursor_x = BATTLE_SELECT_1_X;
  cursor_y = first_line;

  adjust_cursors(selection, first_line);

  int ch;

  flushinp();
  while (1) {
    ch = getch();

    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
      case KEY_DOWN:
        if (selection == 1) selection = 3;
        else if (selection == 2) selection = 4;
        else if (selection == 3) selection = 1;
        else if (selection == 4) selection = 2;
        break;
      case KEY_LEFT:
        if (selection == 1) selection = 4;
         else selection--;
        break;
      case KEY_RIGHT:
         if (selection == 4) selection = 1;
         else selection++;
        break;
      case 'a':
        return (selection);
      default:
        break;
    }
    adjust_cursors(selection, first_line);
  }
}

int get_fight_selection(int first_line, int num_attacks) {
  int selection = 1;
  cursor_x = BATTLE_SELECT_1_X;
  cursor_y = first_line;

  adjust_cursors(selection, first_line);

  int ch;

  flushinp();
  while (1) {
    (ch = getch());

    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
      case KEY_DOWN:
        if (selection == 1 && player.current_pokemon->numAttacks > 2) 
          selection = 3;
        else if (selection == 2 && player.current_pokemon->numAttacks > 3) 
          selection = 4;
        else if (selection == 3) selection = 1;
        else if (selection == 4) selection = 2;
        break;
      case KEY_LEFT:
        if (selection == 1) selection = 5;
        else if (selection == 5) selection = num_attacks;
        else selection--;
        break;
      case KEY_RIGHT:
         if (selection == num_attacks || selection == 4) selection = 5;
         else if (selection == 5) selection = 1;
         else selection++;
        break;
      case 'a':
        return (selection);
      case 'b':
        return (5);
      default:
        break;
    }
    adjust_cursors(selection, first_line);
  }
}

void adjust_cursors(int selection, int first_line) {
  switch (selection) {
    case 1:
      cursor_x = BATTLE_SELECT_1_X; cursor_y = first_line;
      break;
    case 2:
      cursor_x = BATTLE_SELECT_2_X; cursor_y = first_line;
      break;
    case 3:
      cursor_x = BATTLE_SELECT_1_X; cursor_y = first_line+1;
      break;
    case 4:
      cursor_x = BATTLE_SELECT_2_X; cursor_y = first_line+1;
      break;
    case 5:
      cursor_x = BATTLE_SELECT_3_X; cursor_y = first_line+1;
      break;
    default:
      cursor_x = BATTLE_SELECT_3_X; cursor_y = first_line;
      break;
  }

  mvaddch(cursor_y, cursor_x, SELECTION_CHAR);
  refresh();
}


void draw_text_box() {
  drawBox(TEXT_BOX_X, TEXT_BOX_Y, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);
}

static int text_box_cursor_y = 0;

void text_box_cursors(int next_line) {
  if (text_box_cursor_y == 4) next_line = 0;  //Only print 4 lines in text box
  //Clear and reseet if next_line is 0
  if (!next_line) {
    clear_text_box();
    text_box_cursor_y = 0;
  }           
  else text_box_cursor_y++;                   //Increment if next_line is 1
  move(TEXT_BOX_Y+5 + text_box_cursor_y, TEXT_BOX_X+3);
}

void clear_text_box() {
  for (int i = 0; i < 4; i++) {
    mvprintw(TEXT_BOX_Y+5+i, TEXT_BOX_X+3, "\t\t\t\t\t\t");
  }
}

void draw_battle_box() {
  drawBox(BATTLE_BOX_X, BATTLE_BOX_Y, BATTLE_BOX_WIDTH, BATTLE_BOX_HEIGHT);
}