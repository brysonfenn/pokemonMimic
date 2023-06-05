#include "poke_center.h"

#include "monsters/pokemon.h"
#include "monsters/conditions.h"
#include "print/print_utils.h"
#include "print/print_defines.h"
#include "motion/location.h"

#include "player.h"

//Forward Declarations
void handle_PC();
void heal_party();

//Handle all pokemon center functions
int handle_poke_center() {
  int inputNum = 0;

  while (1) {
    begin_list();
    print_to_list("Welcome to the Pokémon Center\n \n");
    print_to_list("  Heal Pokémon\n  PC\n  Exit");
    inputNum = get_selection(2, 2, inputNum);

    clear();
    if (inputNum == 2 || inputNum == PRESSED_B) break;
    else if (inputNum == 0) {
      player.blackout_center->x = player.loc->x;
      player.blackout_center->y = player.loc->y;
      player.blackout_center->map = player.loc->map;
      heal_party();
      break;
    }
    else if (inputNum == 1) {
      handle_PC();
      continue;
    }
  }

  return POKE_CENTER_EXIT;
}


//Handle all Pokemon Storage movement
void handle_PC() {
  int input_num1 = 0;
  int input_num2 = 0;
  char print_str[256];
  
  while (1) {
    begin_list();
    print_to_list("Welcome to the PC! Select a Pokemon from storage below");
    print_pokemon_list(player.pc_storage, player.numInPCStorage);
    print_to_list("  Cancel");

    input_num1 = get_selection(1, player.numInPCStorage, input_num1);
    if (input_num1 == player.numInPCStorage || input_num1 == PRESSED_B) { break; }   //Cancel
    else {
      begin_list();
      print_to_list("  Switch\n  Release\n  Attacks\n  Cancel\n--------------------------------------------------------");
      print_pokemon_summary(&(player.pc_storage[input_num1]));
      input_num2 = get_selection(0, 3, 0);

      //Cancel
      if (input_num2 == 3 || input_num2 == PRESSED_B) { continue; }

      //Release
      else if (input_num2 == 1) {
        begin_list();
        sprintf(print_str, "Are you sure you want to release %s?\n  Yes\n  No\n", player.pc_storage[input_num1].name);
        print_to_list(print_str);
        input_num2 = get_selection(1, 1, 0);

        if (input_num2 == 1 || input_num2 == PRESSED_B) { continue; }
        else {
          sprintf(print_str, " \nBye Bye, %s!\n", player.pc_storage[input_num1].name);
          print_to_list(print_str); sleep(2);
          player.numInPCStorage--;
          for (int i = input_num1; i < player.numInPCStorage; i++) {
            player.pc_storage[i] = player.pc_storage[i+1];
          }
          player.pc_storage[player.numInPCStorage] = emptyPok;
        }
      }

    }
  }
  
  return POKE_CENTER_STAY;
}


//Restore all pokemon to full health, status, PP, etc.
void heal_party() {
  Pokemon * curr_pok;

  begin_list();
  
  for (int i = 0; i < player.numInParty; i++) {
    curr_pok = &(player.party[i]);
    curr_pok->currentHP = curr_pok->maxHP;
    curr_pok->visible_condition = NO_CONDITION;
    remove_all_hidden_conditions(curr_pok);

    for (int j = 0; j < curr_pok->numAttacks; j++) {
      curr_pok->attacks[j].curr_pp = curr_pok->attacks[j].max_pp;
    }
  }
  print_to_list("Your Pokemon were restored to full health!\n \n");
  sleep(2);
  printParty(); sleep(2);
}