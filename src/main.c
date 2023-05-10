#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "print_utils.h"
#include "print_defines.h"
#include "load_save.h"
#include "player.h"
#include "menu.h"

void control_c_handler();

//This is currently the main menu
int main(void) {
  signal(SIGINT, control_c_handler);
  resume_ncurses();
  
  //Initiate Everything
  srand(time(0));
  player_init(0);

  int input_num;
  
  while (1) {
    clear();
    printw("\n  New Game\n  Load Game\n");
    print_btn_instructions(20,0);
    input_num = get_selection(1, 0, 1, 0, MAIN_SELECT);

    //Handle main select returning -1
    if (input_num == -1) {control_c_handler(); return 0;}

    if (input_num == 1) {
      clear();
      printw("Select a save file to load: \n");
      print_save_files();
      input_num = get_selection(1, 0, 9, 0, NOT_MAIN_SELECT);
      clear();
      if (load_game(input_num+1) == LOAD_SUCCESS) {
        break;
      }
    }
    else if (input_num == 0) {
      break;
    }
  }

  main_menu();
  
  //Clear everything and finish
  control_c_handler();
  return 0;
}


void control_c_handler() {
  clear();
  pause_ncurses();

  clearTerminal();
  free(player.loc);

  exit(0);
}