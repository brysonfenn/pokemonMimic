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
#include "print_defines.h"

void clearTerminal() { printf("\033[2J\033[1;1H"); }

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

  flushinp();
  while (1) {
    ch = getch();

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

//Draw simple box
void drawBox(int x, int y, int w, int h) {
    mvaddch(y, x, ACS_ULCORNER);  // Top-left corner
    mvaddch(y, x + w - 1, ACS_URCORNER);  // Top-right corner
    mvaddch(y + h - 1, x, ACS_LLCORNER);  // Bottom-left corner
    mvaddch(y + h - 1, x + w - 1, ACS_LRCORNER);  // Bottom-right corner

    for (int i = x + 1; i < x + w - 1; i++) {
        mvaddch(y, i, ACS_HLINE);  // Top and bottom edges
        mvaddch(y + h - 1, i, ACS_HLINE);
    }

    for (int i = y + 1; i < y + h - 1; i++) {
        mvaddch(i, x, ACS_VLINE);  // Left and right edges
        mvaddch(i, x + w - 1, ACS_VLINE);
    }
}

