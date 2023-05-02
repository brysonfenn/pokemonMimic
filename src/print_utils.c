#include "print_utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <ncurses.h>

#include "monsters/conditions.h"
#include "player.h"
#include "monsters/pokemon.h"
#include "battles/battle.h"

#define SELECTION_CHAR '*'

void adjust_cursors(int selection, int first_line);

void clearTerminal() { printf("\033[2J\033[1;1H"); }

void printBattle() {
  move(0,0);
  pokemon * player_pok = player.current_pokemon;
  pokemon * enemy_pok = player.enemy_pokemon;
  printw("\t\t\t\t%s  Lvl %d  ", enemy_pok->name, enemy_pok->level);
  print_condition(enemy_pok);
  printw("\n\t\t\t\tHP: %d/%d\n\n", enemy_pok->currentHP, enemy_pok->maxHP);
  printw("%s  Lvl %d  ", player_pok->name, player_pok->level);
  print_condition(player_pok);
  printw("\nHP: %d/%d\n\n", player_pok->currentHP, player_pok->maxHP);

  refresh();
}

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines) {
  int inputNum = INVALID_INPUT;
  bool valid = false;
  char input[100];
  char c;
  
  while (!valid) {
    printf("%s", request);
    fgets(input, 100, stdin);
    sscanf(input, "%d", &inputNum);
    
    if (inputNum < beginRange || inputNum > endRange) {
      if (input[0] == '\n') {
        clearLastLine();
        continue;
      }
      sleep(1);
      printf("Please enter a valid number between %d and %d.\n", beginRange, endRange);
      sleep(1);
      for (int i = 0; i < erase_lines; i++) clearLastLine();
      inputNum = INVALID_INPUT;
    }
    else {
      valid = true;
    }
  }
  
  return inputNum;
}

int getValidInput(int beginRange, int endRange, const char * request) {
  return getValidInput_force(beginRange, endRange, request, 2);
}

void clearLastLine() {
  printf("\033[1A"); // Move cursor up one line
  printf("\033[K"); // Clear the line
}

void printParty() {
  printw("Pokemon:\n");
  for (int i = 0; i < player.numInParty; i++) {
    pokemon current_pok = player.party[i];
    int current = current_pok.currentHP;
    int max = current_pok.maxHP;
    printw("  %s\tLVL %d\tHP: %d/%d  ", current_pok.name, current_pok.level, current, max);
    if (!(current)) printw(" (Fainted) ");
    print_condition(&current_pok);
    printw("\n");
  }
  refresh();
}

void printBag() {
  printw("Bag:\n");
  for (int i = 0; i < player.numInBag; i++) {
    mvprintw(i+1,0, "  %s", player.bag[i].name);
    mvprintw(i+1,20, "%d", player.bag[i].number);
  }
  printw("\n  Cancel\n\n");
  refresh();
}

void resume_ncurses() {
  initscr(); // Initialize ncurses
  cbreak(); // Disable line buffering
  noecho(); // Don't display typed characters
  keypad(stdscr, true); // Enable arrow keys
  clear();
  curs_set(0);
  start_color();
}


void pause_ncurses() {
  endwin(); // Clean up ncurses
  setvbuf(stdout, NULL, _IOLBF, 0);
  flushinp();
}

int get_selection(int first_line, int start, int end, int last_selection) {
  int cursor_x = 0;
  int cursor_y = first_line + last_selection;
  mvaddch(cursor_y, cursor_x, SELECTION_CHAR);
  refresh();

  int ch;
  while ((ch = getch()) != 'q') {
    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
        if (cursor_y == first_line + start) cursor_y = first_line + end;
        else cursor_y--;
        break;
      case KEY_DOWN:
        if (cursor_y == first_line + end) cursor_y = first_line + start;
        else cursor_y++;
        break;
      case KEY_LEFT:
        break;
      case KEY_RIGHT:
        break;
      case 'a':
        return (cursor_y - first_line);
      case 'b':
        return (end);
      default:
        break;
    }
    mvaddch(cursor_y, cursor_x, SELECTION_CHAR);
    refresh();
  }

  return -1;

}


static int cursor_x;
static int cursor_y;

int get_battle_selection(int first_line, int last_selection) {
  int selection = last_selection;
  cursor_x = BATTLE_SELECT_1_X;
  cursor_y = first_line;

  adjust_cursors(selection, first_line);

  int ch;
  while ((ch = getch()) != 'q') {
    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
        break;
      case KEY_DOWN:
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
  while ((ch = getch()) != 'q') {
    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
        break;
      case KEY_DOWN:
        break;
      case KEY_LEFT:
        if (selection == 1) selection = 5;
        else if (selection == 5) selection = num_attacks;
        else selection--;
        break;
      case KEY_RIGHT:
         if (selection == num_attacks) selection = 5;
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