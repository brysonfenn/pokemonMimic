#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "print_utils.h"
#include "load_save.h"
#include "motion2d.h"

#include "monsters/pokemon.h"
#include "player.h"
#include "battles/battle.h"
#include "battles/trainer.h"
#include "battles/wild_pokemon.h"

static enum display { MAIN, POWER_OFF, WILD, TRAINER, POKEMON, BAG, PLAYER, SAVE, LOAD,
    TOWN } current_display = MAIN;

pokemon tempPok;

static bool power_off = false;

void control_c_handler();

//This is currently the main menu
int main(void) {

  signal(SIGINT, control_c_handler);

  int inputNum, inputNum2;
  char example_string[100];
  int return_execute;

  //Initiate Everything
  player_init(0);
  items_init();
  attacks_init();
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
      printf("1: Wild Pokemon\n2: Trainer\n3: Pokemon\n4: Bag\n5: Player\n");
      printf("6: Save Game\n7: Load Game\n8: Town\n0: Power Off\n\n");
      inputNum = getValidInput(0, 10, "Select an Option: ");
      fflush(stdout);
      current_display = inputNum + 1; //Adjust to fit enum position
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
      printParty();
      printf("%d: Cancel\n\n", 0);
      inputNum = getValidInput(0, player.numInParty, "Select a Pokemon: ");
      if (!inputNum) { current_display = MAIN; break; }
      inputNum--; //Adjust input number to array position

      clearTerminal();
      print_pokemon_summary(&(player.party[inputNum]));
      printf("\n1: Switch\n2: Release\n0: Cancel\n");
      inputNum2 = getValidInput_force(0, 2, "Select an Option: ", 6);

      //Break if inputNum2 is 0 (cancel)
      if (!inputNum2) { break; } 
      //Switch
      else if (inputNum2 == 1) {
        clearTerminal();
        printParty();
        printf("\n");
        sprintf(example_string, "Select a Pokemon to switch with %s: ", player.party[inputNum].name);
        inputNum2 = getValidInput(1, player.numInParty, example_string);
        inputNum2--; //Adjust input number to array position

        tempPok = player.party[inputNum];
        player.party[inputNum] = player.party[inputNum2];
        player.party[inputNum2] = tempPok;
      }
      //Release
      else if (inputNum2 == 2) {
        if (player.numInParty <= 1) {printf("You only have 1 Pokémon!\n"); sleep(2); break; }
        clearTerminal();
        printf("Are you sure you want to release %s?\n1:Yes\n0:No\n", player.party[inputNum].name);
        inputNum2 = getValidInput(0, 1, "Select an option: ");
        if (!inputNum2) { break; }
        else {
          printf("Bye Bye, %s!\n", player.party[inputNum].name); sleep(2);
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
      printBag();
      inputNum = getValidInput(0, player.numInBag, "Select an item to use: ");
      if (!inputNum) { current_display = MAIN; break; }
      inputNum--; //Adjust input number to array position
      return_execute = use_item(inputNum, &emptyPok);

      //Return to bag menu if item failed, else back to main menu
      if (return_execute == ITEM_FAILURE || return_execute == ITEM_CATCH_FAILURE) { continue; }
      current_display = MAIN;
      break;
    
    //Display player data
    case PLAYER:
      printPlayer();
      sleep(3);
      current_display = MAIN;
      break;

    //Save game data to a file
    case SAVE:
      print_save_files();
      inputNum = getValidInput(0, 10, "Enter a save file to save to: ");
      if (inputNum == 0) { current_display = MAIN; break; }
      save_game(inputNum);
      current_display = MAIN;
      break;

    //Load game data from a file
    case LOAD:
      print_save_files();
      inputNum = getValidInput(0, 10, "Enter a save file to load: ");
      if (inputNum == 0) { current_display = MAIN; break; }
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
      printf("Ran into default in main loop\n"); sleep(1);
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