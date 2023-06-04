#include "print_utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <ncurses.h>
#include <locale.h>

#include "../monsters/conditions.h"
#include "../player.h"
#include "../monsters/pokemon.h"
#include "../battles/battle.h"
#include "../items.h"
#include "../motion/map_drawing.h"

#include "print_defines.h"


//Print a list of Pokemon (used for party and PC)
void print_pokemon_list(struct Pokemon * pokList, int list_size) {
  char list_str[8192] = "";

  for (int i = 0; i < list_size; i++) {
    Pokemon current_pok = pokList[i];
    int current = current_pok.currentHP;
    int max = current_pok.maxHP;
    sprintf(list_str, "%s  %s" , list_str, current_pok.name);

    //handle spacing
    for (int j = strlen(current_pok.name); j < 15; j++) sprintf(list_str, "%s ", list_str);

    sprintf(list_str, "%sLVL %d\tHP: %d/%d ", list_str, current_pok.level, current, max);
    if (!(current)) sprintf(list_str, "%s (Fainted) ", list_str);
    add_condition_string(list_str, &current_pok);
    sprintf(list_str, "%s\n", list_str);
  }
  print_to_list(list_str);
}


//Print the player's current pokemon party
void printParty() {
  print_to_list("Pokemon:\n");
  print_pokemon_list(player.party, player.numInParty);
}

//Print the player's current bag
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

//Initialize ncurses library - including colors, extended character library, etc.
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
  init_pair(HEAL_COLOR, COLOR_BLUE, COLOR_BLACK);
  init_pair(TREE_COLOR, COLOR_RED, COLOR_GREEN);
}

//End ncurses library functionality
void pause_ncurses() {
  endwin(); // Clean up ncurses
  setvbuf(stdout, NULL, _IOLBF, 0);
  flushinp();
}

//Get player selection from list
//  first_line is the first line of the list (0 is the top of the terminal)
//  num_options is the number of options in the list available to select
//  last_selection is where the cursor will begin (usually 0 or the last option selected)
int get_selection(int first_line, int num_options, int last_selection) {

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
        if (cursor_y == first_line) cursor_y = first_line + num_options;
        else cursor_y--;
        break;
      case KEY_DOWN:
        if (cursor_y == first_line + num_options) cursor_y = first_line;
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

//Draw simple box with top-left corner (x,y) and with width and height (w,h)
void draw_box(int x, int y, int w, int h) {
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

//Print the instruction box at a given location (x,y)
//  If on_map is true, the special instruction to go to menu will be shown
void print_btn_instructions(int x, int y, bool on_map) {
  draw_box(x,y,15,7);
  mvprintw(y+1,x+3, "a: Select");
  mvprintw(y+2,x+3, "b: Cancel");
  if (on_map) mvprintw(y+3,x+3, "m: menu");

  // Print the arrow symbols
  mvprintw(y+4,x+2, "  \u2191");
  mvprintw(y+5,x+2, "\u2190 \u2193 \u2192 Move");

  refresh();
}


//List functions
int list_item_num = 0;

//Erase current list and resetart at the beginning
void begin_list() {
  list_item_num = 0;
  clear();
  draw_box(LIST_BOX_X, LIST_BOX_Y, LIST_BOX_WIDTH, LIST_BOX_HEIGHT);
  
  //print button instructons where they will be in battle
  print_btn_instructions(LIST_BOX_X+LIST_BOX_WIDTH+2, TEXT_BOX_Y, false);
  refresh();
}

//Print to the next line of the list
// list_str must be a string constant, and end_line characters are understood
// to indicate a next list item
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


#define ROWS 30
#define COLS 100
static int contents[ROWS][COLS];

//Save print state to print later
void save_print_state() {
  move(0,0);
  for (int row = 0; row < ROWS; ++row) {
    for (int col = 0; col < COLS; ++col) {
      contents[row][col] = inch();
      move(row, col + 1);
    }
  }
}

//Reprint the saved print state - cannot be called without first calling save_print_state
void restore_print_state() {
  clear(); 
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      addch(contents[row][col]);
      move(row, col + 1);
    }
  }
  refresh();
}