#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "print_utils.h"
#include "load_save.h"
#include "motion2d.h"

#include "pokemon.h"
#include "player.h"
#include "battle.h"
#include "trainer.h"

#define TOTAL_POKE_NUM 3

static enum display { MAIN, WILD, TRAINER, POKECENTER, POKEMON, BAG, PLAYER, SAVE, LOAD, MART, BACK, 
  POWER_OFF } current_display = MAIN;

pokemon wild_pokemon[TOTAL_POKE_NUM];
pokemon tempPok;

static bool power_off = false;

void control_c_handler();

int main(void) {

  signal(SIGINT, control_c_handler);

  int inputNum, inputNum2;
  pokemon example_pokemon;
  char example_string[100];
  int return_execute;

  //Initiate Everything
  pokemon_init();
  player_init();
  items_init();
  attacks_init();
  srand(time(0));

  wild_pokemon[0] = caterpie;
  wild_pokemon[1] = charmander;
  wild_pokemon[2] = squirtle;


  while(1) {
    clearTerminal();
    for (int i = 0; i < player.numInParty; i++) {
      reset_base_stats(&(player.party[i]));
    }

    switch (current_display) {
    case MAIN:
      printf("0: Wild Pokemon\n1: Trainer  \n2: PokeCenter\n3: Pokemon  \n4: Bag    \n5: Player\n");
      printf("6: Save Game \n7: Load Game\n8: Mart \n9: Back\n10: Power Off\n\n");
      inputNum = getValidInput(0, 10, "Select an Option: ");
      fflush(stdout);
      current_display = inputNum + 1;
      break;
    case WILD:
      player.trainer_battle = false;
      set_current_pokemon(PLAYER_DEFAULT_POKEMON);
      if (player.numAlive) {
        example_pokemon = *(get_random_wild_pokemon(2, 4));
        // example_pokemon = sandshrew;
        clearTerminal();
        printf("A wild %s appeared!\n", example_pokemon.name);
        sleep(3);
        initiate_battle(example_pokemon);
      }
      else {
        printf("All Pokemon have fainted, please heal them.\n");
        sleep(3);
      }
      current_display = MAIN;
      break;
    case TRAINER:
      player.trainer_battle = true;
      if (player.numAlive) {
        battleTrainer();
      }
      else {
        printf("All Pokemon have fainted, please heal them.\n");
        sleep(3);
      }
      current_display = MAIN;
      break;
    case POKECENTER:
      heal_party();
      printParty();
      sleep(2);
      current_display = MAIN;
      break;
    case POKEMON:
      printParty();
      printf("%d: Cancel\n\n", player.numInParty);
      inputNum = getValidInput(0, player.numInParty, "Select a Pokemon: ");

      if (inputNum != player.numInParty) {
        clearTerminal();
        print_pokemon_summary(&(player.party[inputNum]));
        inputNum2 = getValidInput_force(0, 2, "\n0: Cancel\n1: Switch\n2: Release\nSelect an Option: ", 6);

        if (!inputNum2) { break; } //Break if inputNum2 is 0 (cancel)
        else if (inputNum2 == 1) {
          clearTerminal();
          printParty();
          printf("\n");
          sprintf(example_string, "Select a Pokemon to switch with %s: ", player.party[inputNum].name);
          inputNum2 = getValidInput(0, player.numInParty - 1, example_string);
          tempPok = player.party[inputNum];
          player.party[inputNum] = player.party[inputNum2];
          player.party[inputNum2] = tempPok;
        }
        else if (inputNum2 == 2) {
          if (player.numInParty <= 1) {printf("You only have 1 Pokémon!\n"); sleep(2); break; }
          clearTerminal();
          printf("Are you sure you want to release %s?\n0:Yes\n1:No\n", player.party[inputNum].name);
          inputNum2 = getValidInput(0, 1, "Select an option: ");
          if (inputNum2) { break; }
          else {
            printf("Bye Bye, %s!\n", player.party[inputNum].name); sleep(2);
            player.numInParty--;
            for (int i = inputNum; i < player.numInParty; i++) {
              player.party[i] = player.party[i+1];
            }
            player.party[player.numInParty] = emptyPok;
          }
        }
      }
      current_display = MAIN;
      break;
    case BAG:
      printBag();
      inputNum = getValidInput(0, player.numInBag, "Select an item to use: ");
      if (inputNum == player.numInBag) { current_display = MAIN; break; }
      return_execute = use_item(inputNum, &emptyPok);

      if (return_execute == ITEM_FAILURE || return_execute == ITEM_CATCH_FAILURE) { continue; }
      current_display = MAIN;
      break;
    case PLAYER:
      printPlayer();
      sleep(3);
      current_display = MAIN;
      break;
    case SAVE:
      print_save_files();
      inputNum = getValidInput(0, 10, "Enter a save file to save to: ");
      if (inputNum == 0) { current_display = MAIN; break; }
      save_game(inputNum);
      current_display = MAIN;
      break;
    case LOAD:
      print_save_files();
      inputNum = getValidInput(0, 10, "Enter a save file to load: ");
      if (inputNum == 0) { current_display = MAIN; break; }
      load_game(inputNum);
      current_display = MAIN;
      break;
    case MART:
      if (handle_mart() == ITEM_FAILURE) { continue; }
      current_display = MAIN;
      break;
    case BACK:
      handle_motion();
      current_display = MAIN;
      break;
    case POWER_OFF:
      power_off = true;
      current_display = MAIN;
      break;
    default:
      printf("Ran into default in main loop\n");
      break;
    }

    if (power_off) break;
  }

  //Clear everything and finish
  clearTerminal();

  return 0;
}


void control_c_handler() {
  clearTerminal();
  exit(0);
}