#include "print_utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <ncurses.h>
#include <locale.h>

#include "monsters/conditions.h"
#include "player.h"
#include "monsters/pokemon.h"
#include "battles/battle.h"
#include "print_defines.h"
#include "items.h"
#include "motion/map_drawing.h"

void clearTerminal() { printf("\033[2J\033[1;1H"); }

void clearLastLine() {
  printf("\033[1A"); // Move cursor up one line
  printf("\033[K"); // Clear the line
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

void printParty() {
  char party_str[1024] = "";

  sprintf(party_str, "%sPokemon:\n", party_str);
  for (int i = 0; i < player.numInParty; i++) {
    pokemon current_pok = player.party[i];
    int current = current_pok.currentHP;
    int max = current_pok.maxHP;
    sprintf(party_str, "%s  %s" , party_str, current_pok.name);

    //handle spacing
    for (int j = strlen(current_pok.name); j < 15; j++) sprintf(party_str, "%s ", party_str);

    sprintf(party_str, "%sLVL %d\tHP: %d/%d ", party_str, current_pok.level, current, max);
    if (!(current)) sprintf(party_str, "%s (Fainted) ", party_str);
    add_condition(party_str, &current_pok);
    sprintf(party_str, "%s\n", party_str);
  }
  print_to_list(party_str);
  refresh();
}

void printBag() {
  char bag_str[1024] = "";
  sprintf(bag_str, "%sBag:\n", bag_str);
  for (int i = 0; i < player.numInBag; i++) {
    sprintf(bag_str, "%s  %s", bag_str, player.bag[i].name);

    //Space number properly
    for (int j = strlen(player.bag[i].name); j < 20; j++) sprintf(bag_str, "%s ", bag_str);
    sprintf(bag_str, "%s%d\n", bag_str, player.bag[i].number);
  }
  sprintf(bag_str, "%s\n  Cancel", bag_str);
  print_to_list(bag_str);
  refresh();
}

void resume_ncurses() {

  //Allow unicode symbols (like arrow keys)
  setlocale(LC_ALL, "");
  setenv("NCURSES_UNICODE", "1", 1);

  //Initialize
  initscr(); // Initialize ncurses
  cbreak(); // Disable line buffering
  noecho(); // Don't display typed characters
  keypad(stdscr, true); // Enable arrow keys
  clear();
  curs_set(0);

  //Color
  start_color();
  // Define color pairs
  init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(PLAYER_COLOR, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(GRASS_COLOR, COLOR_GREEN, COLOR_BLACK);
  init_pair(DAMAGED_COLOR, COLOR_RED, COLOR_BLACK);
  init_pair(TRAINER_COLOR, COLOR_RED, COLOR_BLACK);
}


void pause_ncurses() {
  endwin(); // Clean up ncurses
  setvbuf(stdout, NULL, _IOLBF, 0);
  flushinp();
}

int get_selection(int first_line, int start, int end, int last_selection) {

  int cursor_x = LIST_BOX_X+1;
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
        return (PRESSED_B);
        break;
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


void print_btn_instructions(int x, int y) {
  drawBox(x,y,15,7);
  mvprintw(y+1,x+3, "a: Select");
  mvprintw(y+2,x+3, "b: Cancel");

  // Print the arrow symbols
  mvprintw(y+4,x+2, "  \u2191");
  mvprintw(y+5,x+2, "\u2190 \u2193 \u2192 Move");

  refresh();
}

int list_item_num = 0;

void begin_list() {
  list_item_num = 0;
  clear();
  drawBox(LIST_BOX_X, LIST_BOX_Y, LIST_BOX_WIDTH, LIST_BOX_HEIGHT);
  
  //print button instructons where they will be in battle
  print_btn_instructions(LIST_BOX_X+LIST_BOX_WIDTH+2, TEXT_BOX_Y);
  refresh();
}

void print_to_list(const char * list_str) {
  char * token;
  char* input = strdup(list_str); //input is mutable version of input str

  if (input == NULL) {
    // Error handling for memory allocation failure
    mvprintw(2, 2, "Failed to allocate memory for printing to list."); refresh(); sleep(2); 
    return;
  }

  // Tokenize the string by endlines
  token = strtok(input, "\n");
  while (token != NULL) {
    mvprintw(LIST_BOX_Y+1+list_item_num, LIST_BOX_X+ 1, "%s", token);
    token = strtok(NULL, "\n");
    list_item_num++;
  }
  refresh();
}