#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>

#include "print/print_utils.h"
#include "print/print_defines.h"
#include "load_save.h"
#include "player.h"
#include "monsters/pokemon.h"
#include "menu.h"
#include "test.h"

void control_c_handler();

//Main function
int main(void) {

  signal(SIGINT, control_c_handler);
  resume_ncurses();
  
  //Initiate Everything
  srand(time(NULL));
  player_init(0);

  int input_num;
  
  while (1) {
    begin_list();
    print_to_list("  New Game\n  Load Game\n");

    //                                          // To Test:
    // print_to_list("  Test");                 // Uncomment
    // input_num = get_selection(0, 2, 0);      // Uncomment

    input_num = get_selection(0, 1, 0);         // Comment

    //Handle main select returning -1
    if (input_num == PRESSED_B) {control_c_handler(); return 0;}

    if (input_num == 1) {
      begin_list();
      print_to_list("Select a save file to load: \n");
      print_save_files();
      input_num = get_selection(1, 9, 0);
      
      //Get proper input
      if (input_num == 9 || input_num == PRESSED_B) {
        continue;
      }
      if (load_game(input_num+1) == LOAD_SUCCESS) {
        break;
      }
    }
    else if (input_num == 0) {
      begin_list();
      print_to_list("Select a starter Pokémon: \n  Bulbasaur\n  Charmander\n  Squirtle\n  Cancel");
      input_num = get_selection(1, 3, 0);
      if (input_num == PRESSED_B || input_num == 3) {
        continue;
      }
      
      player.numInParty = 1;
      player.party[0] = *(get_starter(input_num));  //Give the player a starter based on selection

      break;
    }
    else if (input_num == 2) {
      while (1) {
        test_function();
        begin_list();
        print_to_list("  \n  End Test"); sleep(2);
      }
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