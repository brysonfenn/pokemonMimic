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
#include "../items/items.h"
#include "../motion/map_drawing.h"

#include "print_defines.h"


static char name_input_str[30] = "";


//Print a list of Pokemon (used for party and PC)
void print_pokemon_list(struct Pokemon * pokList, int list_size) {
  char list_str[8192] = "";

  for (int i = 0; i < list_size; i++) {
    Pokemon current_pok = pokList[i];
    int current = current_pok.currentHP;
    int max = current_pok.maxHP;
    sprintf(list_str, "%s  %s" , list_str, current_pok.name);

    //Handle spacing
    for (int j = strlen(current_pok.name); j < 15; j++) sprintf(list_str, "%s ", list_str);
    //Fix Nidoran male/female symbol spacing
    bool is_nidoran = ((current_pok.id_num == POKEMON_NIDORAN_F) || (current_pok.id_num == POKEMON_NIDORAN_M));
    if (is_nidoran) sprintf(list_str, "%s  ", list_str);

    //Add other pokemon info
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


//Print the player's party, with able/unable based on size [6] bool array
void print_party_able_unable(const bool able_array[6]) {
  char list_str[8192] = "";

  for (int i = 0; i < player.numInParty; i++) {
    Pokemon current_pok = player.party[i];
    sprintf(list_str, "%s  %s" , list_str, current_pok.name);

    //Handle spacing
    for (int j = strlen(current_pok.name); j < 15; j++) sprintf(list_str, "%s ", list_str);
    //Fix Nidoran male/female symbol spacing
    bool is_nidoran = ((current_pok.id_num == POKEMON_NIDORAN_F) || (current_pok.id_num == POKEMON_NIDORAN_M));
    if (is_nidoran) sprintf(list_str, "%s  ", list_str);

    //Add able/unable info
    char able_unable[32];
    if (able_array[i]) sprintf(able_unable, "%s", "Able");
    else sprintf(able_unable, "%s", "Unable");
    sprintf(list_str, "%s%s\n", list_str, able_unable);
  }
  print_to_list(list_str);
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
  init_pair(WATER_COLOR, COLOR_BLUE, COLOR_BLACK);
  init_pair(NPC_COLOR, COLOR_BLUE, COLOR_BLACK);
  init_pair(INVERSE_COLOR, COLOR_BLACK, COLOR_WHITE);
}

//End ncurses library functionality
void pause_ncurses() {
  endwin(); // Clean up ncurses
  setvbuf(stdout, NULL, _IOLBF, 0);
  flushinp();
}

//Get player selection from list
//  first_line is the first line of the list (0 is the top of the terminal)
//  highest_option_num is the number of options in the list available to select
//  last_selection is where the cursor will begin (usually 0 or the last option selected)
int get_selection(int first_line, int highest_option_num, int last_selection) {

  int cursor_x = LIST_BOX_X+1;
  // The actual first line should be adjusted to the list box position
  int actual_first_line = LIST_BOX_Y + first_line + 1;
  int cursor_y = actual_first_line + last_selection;
  mvaddch(cursor_y, cursor_x, SELECTION_CHAR);
  refresh();

  int ch;

  flushinp();
  while (1) {
    ch = getch();

    mvaddch(cursor_y, cursor_x, ' ');

    switch (ch) {
      case KEY_UP:
        if (cursor_y == actual_first_line) cursor_y = actual_first_line + highest_option_num;
        else cursor_y--;
        break;
      case KEY_DOWN:
        if (cursor_y == actual_first_line + highest_option_num) cursor_y = actual_first_line;
        else cursor_y++;
        break;
      case SELECT_CHAR:
        return (cursor_y - actual_first_line);
      case CANCEL_CHAR:
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
void print_btn_instructions(bool on_map) {

  draw_box(BTN_INSTRUCTIONS_X,BTN_INSTRUCTIONS_Y,15,7);
  mvprintw(BTN_INSTRUCTIONS_Y+1,BTN_INSTRUCTIONS_X+3, "%c: Select", SELECT_CHAR);
  mvprintw(BTN_INSTRUCTIONS_Y+2,BTN_INSTRUCTIONS_X+3, "%c: Cancel", CANCEL_CHAR);
  if (on_map) mvprintw(BTN_INSTRUCTIONS_Y+3,BTN_INSTRUCTIONS_X+3, "%c: menu", MENU_CHAR);

  // Print the arrow symbols
  mvprintw(BTN_INSTRUCTIONS_Y+4,BTN_INSTRUCTIONS_X+2, "  \u2191");
  mvprintw(BTN_INSTRUCTIONS_Y+5,BTN_INSTRUCTIONS_X+2, "\u2190 \u2193 \u2192 Move");

  //Await User box
  draw_box(AWAIT_USER_X, AWAIT_USER_Y, 15, 3);

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
  print_btn_instructions(false);
  refresh();
}

//Wait for user to press 'a' button ('z')
int await_user() {
  usleep(500000);
  int ch = '~';
  flushinp();
  mvprintw(AWAIT_USER_Y+1, AWAIT_USER_X+1, "Press '%c'", SELECT_CHAR); refresh();
  while (ch != SELECT_CHAR) ch = getch();

  mvprintw(AWAIT_USER_Y+1, AWAIT_USER_X+1, "           "); refresh();
}

//Insert end lines to avoid text box overflow
void fix_list_box_overflow(char * input) {
  //Get the first token and check length
  int input_length = strlen(input);
  int count = 0;
  
  for (int i = 0; i < input_length; i++) {
    //Add endlines if there are too many characters between two '\n'
    if (input[i] == '\n') {
      count = 0; continue;
    }
    else if (count >= LIST_BOX_WIDTH-3) {
      int j;
      for (j = i; j > i-LIST_BOX_WIDTH+2; j--) {
        if (input[j] == ' ') break;
      }
      input[j] = '\n';
      count = i - j;  //Reset count to where it should beb
    }
    else {
      count++;
    }
  }
  
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

  fix_list_box_overflow(input);

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


//Get name input from user
char * get_name_input(char * target_for_name) {
  char curr_char = ' ';
  int ch, string_length = 0;
  print_alphabet(curr_char);
  sprintf(name_input_str, "");
  mvprintw(MAP_Y+2, MAP_X+1, "Name %s: [%s]", target_for_name, name_input_str);

  while (1) {
    ch = getch();

    switch (ch) {
      case KEY_UP:
        if (curr_char - 14 >= ' ') curr_char -= 14;
        break;
      case KEY_DOWN:
        if (curr_char + 14 > 'z' && curr_char <= 's') curr_char = INPUT_NAME_DONE_CHAR;
        else if (curr_char + 14 <= 'z') curr_char += 14;
        break;
      case KEY_LEFT:
        if (curr_char == 't') curr_char = INPUT_NAME_DONE_CHAR;
        else if ((curr_char - ' ') % 14 == 0) curr_char += 13;
        else curr_char--;
        break;
      case KEY_RIGHT:
        if (curr_char == INPUT_NAME_DONE_CHAR) curr_char = 't';
        else if (curr_char == 'z') curr_char = INPUT_NAME_DONE_CHAR;
        else if ((curr_char - ' ') % 14 == 13) curr_char -= 13;
        else curr_char++;
        break;
      case SELECT_CHAR:
        string_length = strlen(name_input_str);
        //Handle done
        if (string_length > 0 && curr_char == INPUT_NAME_DONE_CHAR) {
          return name_input_str;
        }
        //Add letter to the string
        else if (string_length < 30) {
          name_input_str[string_length] = curr_char;
          name_input_str[string_length+1] = '\0';
        }
          break;
      case CANCEL_CHAR:
        string_length = strlen(name_input_str);
        if (string_length > 0) {
          name_input_str[string_length-1] = '\0';
        }
        break;
      default:
        break;  
    }

    print_alphabet(curr_char);
    mvprintw(MAP_Y+2, MAP_X+1, "Name %s: [%s]", target_for_name, name_input_str);
  }
}

//Print all typable letters
void print_alphabet(char curr_char) {
  int curr_x = ALPHABET_BEGIN_X;
  int curr_y = ALPHABET_BEGIN_Y;
  char c = ' ';

  begin_list();

  while (c <= INPUT_NAME_DONE_CHAR) {
    if (c == curr_char) attrset(COLOR_PAIR(INVERSE_COLOR));

    if (c == INPUT_NAME_DONE_CHAR) mvprintw(curr_y, MAP_X+MAP_WIDTH-10, "Done");
    else mvprintw(curr_y, curr_x, "%c", c);
    
    curr_x += 4;
    if (curr_x >= MAP_X+MAP_WIDTH) {
      curr_x = ALPHABET_BEGIN_X;
      curr_y += 2;
    }
    if (c == curr_char) attrset(COLOR_PAIR(DEFAULT_COLOR));
    c++;
  }
}