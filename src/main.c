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
    begin_list();
    print_to_list("  New Game\n  Load Game\n");
    input_num = get_selection(LIST_BOX_Y+1, 0, 1, 0);

    //Handle main select returning -1
    if (input_num == PRESSED_B) {control_c_handler(); return 0;}

    if (input_num == 1) {
      begin_list();
      print_to_list("Select a save file to load: \n");
      print_save_files();
      input_num = get_selection(LIST_BOX_Y+2, 0, 9, 0);
      
      //Get proper input
      if (input_num == 9 || input_num == PRESSED_B) {
        continue;
      }
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