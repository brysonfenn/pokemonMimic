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

#define SELECTION_CHAR '*'

void clearTerminal() { printf("\033[2J\033[1;1H"); }

void printBattle() {
  pokemon * player_pok = player.current_pokemon;
  pokemon * enemy_pok = player.enemy_pokemon;
  printf("\t\t\t\t%s  Lvl %d  ", enemy_pok->name, enemy_pok->level);
  print_condition(enemy_pok);
  printf("\n\t\t\t\tHP: %d/%d\n\n", enemy_pok->currentHP, enemy_pok->maxHP);
  printf("%s  Lvl %d  ", player_pok->name, player_pok->level);
  print_condition(player_pok);
  printf("\nHP: %d/%d\n\n", player_pok->currentHP, player_pok->maxHP);
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

    //If the user entered a number greater than they should, get the lowest digit.
    if (inputNum > endRange) { inputNum = inputNum % 10; }
    
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
    printw("  %s\t%d\n", player.bag[i].name, player.bag[i].number);
  }
  printw("  Cancel\n\n");
}

void resume_ncurses() {
  initscr(); // Initialize ncurses
  cbreak(); // Disable line buffering
  noecho(); // Don't display typed characters
  keypad(stdscr, true); // Enable arrow keys
  clear();
  curs_set(0);
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
        clear();
        return (cursor_y - first_line);
      default:
        break;
    }
    mvaddch(cursor_y, cursor_x, SELECTION_CHAR);
    refresh();
  }

}