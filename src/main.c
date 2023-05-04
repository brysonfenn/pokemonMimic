#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "print_utils.h"
#include "load_save.h"
#include "maps/motion2d.h"

#include "player.h"
#include "monsters/pokemon.h"
#include "battles/battle.h"
#include "battles/trainer.h"
#include "battles/wild_pokemon.h"

static enum display { WILD, TRAINER, POKEMON, BAG, PLAYER, SAVE, LOAD,
    TOWN, POWER_OFF, MAIN } current_display = MAIN;

pokemon tempPok;

static bool power_off = false;

void control_c_handler();

//This is currently the main menu
int main(void) {

  signal(SIGINT, control_c_handler);

  int inputNum, inputNum2, return_execute;
  char example_string[100];
  int last_selection = 0;

  //Initiate Everything
  player_init(0);
  items_init();
  srand(time(0));

  //This is the main while loop.
  while(1) {
    clearTerminal();
    for (int i = 0; i < player.numInParty; i++) {
      reset_base_stats(&(player.party[i]));
    }

    //Handle changing the display
    switch (current_display) {

    //This is the actual main menu
    case MAIN:
      resume_ncurses();
      printw("  Wild Pokemon\n  Trainer\n  Pokemon\n  Bag\n  Player\n");
      printw("  Save Game\n  Load Game\n  Town\n  Power Off\n\n");

      inputNum = get_selection(0,0,8,last_selection);
      last_selection = inputNum;
      fflush(stdout);
      current_display = inputNum;
      clear();
      break;

    //Battle wild pokemon
    case WILD:
      battle_wild_pokemon();
      current_display = MAIN;
      break;

    //Battle a trainer
    case TRAINER:
      battle_trainer();
      current_display = MAIN;
      break;

    //Handle party changes, releases, and viewing stats
    case POKEMON:
      clear();
      printParty();
      printw("  Cancel\n\n", 0);
      inputNum = get_selection(1,0,player.numInParty,0);
      if (inputNum == player.numInParty) { current_display = MAIN; break; }
      

      clear();
      print_pokemon_summary(&(player.party[inputNum]));
      printw("\n  Switch\n  Release\n  Cancel\n");
      inputNum2 = get_selection(12, 0, 2, 0);

      //Break if inputNum2 is 2 (cancel)
      if (inputNum2 == 2) { break; } 
      //Switch
      else if (inputNum2 == 0) {
        if (player.numInParty <= 1) {printw("You only have 1 Pokémon!\n"); refresh(); sleep(2); break; }
        clear();
        printw("Which pokemon would you like to switch with %s?\n", player.party[inputNum].name);
        printParty();
        printw("\n");
        inputNum2 = get_selection(2,0,player.numInParty-1,0);

        tempPok = player.party[inputNum];
        player.party[inputNum] = player.party[inputNum2];
        player.party[inputNum2] = tempPok;
      }
      //Release
      else if (inputNum2 == 1) {
        if (player.numInParty <= 1) {printw("You only have 1 Pokémon!\n"); refresh(); sleep(2); break; }
        clear();
        printw("Are you sure you want to release %s?\n  Yes\n  No\n", player.party[inputNum].name);
        inputNum2 = get_selection(1,0,1,0);
        if (inputNum2) { break; }
        else {
          clear();
          printw("Bye Bye, %s!\n", player.party[inputNum].name); refresh(); sleep(2);
          player.numInParty--;
          for (int i = inputNum; i < player.numInParty; i++) {
            player.party[i] = player.party[i+1];
          }
          player.party[player.numInParty] = emptyPok;
        }
      }
      current_display = MAIN;
      break;

    //Handle items used by player
    case BAG:
      clear();
      printBag();
      inputNum = get_selection(1,0,player.numInBag,0);
      if (inputNum == player.numInBag) { current_display = MAIN; break; }
      return_execute = use_item(inputNum, &emptyPok);

      //Return to bag menu if item failed, else back to main menu
      if (return_execute == ITEM_FAILURE || return_execute == ITEM_CATCH_FAILURE) { continue; }
      current_display = MAIN;
      break;
    
    //Display player data
    case PLAYER:
      printPlayer();
      current_display = MAIN;
      break;

    //Save game data to a file
    case SAVE:
      pause_ncurses();
      print_save_files();
      inputNum = getValidInput(0, 10, "Enter a save file to save to: ");
      clearTerminal();
      if (inputNum == 0) { current_display = MAIN; break; }
      resume_ncurses();
      save_game(inputNum);
      current_display = MAIN;
      break;

    //Load game data from a file
    case LOAD:
      pause_ncurses();
      print_save_files();
      inputNum = getValidInput(0, 10, "Enter a save file to load: ");
      clearTerminal();
      if (inputNum == 0) { current_display = MAIN; break; }
      resume_ncurses();
      load_game(inputNum);
      current_display = MAIN;
      break;
    
    //Allow player to walk around. PokeCenter and Mart are here
    case TOWN:
      handle_motion();
      current_display = MAIN;
      break;

    //Allow loop to break and end game
    case POWER_OFF:
      power_off = true;
      current_display = MAIN;
      break;

    //This should never happen.
    default:
      printw("Ran into default in main loop\n"); refresh(); sleep(1);
      break;
    }

    if (power_off) break;
  }

  //Clear everything and finish
  control_c_handler();

  return 0;
}


void control_c_handler() {
  clearTerminal();
  endwin(); // Clean up ncurses
  setvbuf(stdout, NULL, _IOLBF, 0);

  free(player.loc);

  exit(0);
}