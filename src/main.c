#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "print_utils.h"
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