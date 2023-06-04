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
  int inputNum;
  begin_list();
  print_to_list("Welcome to the Pokémon Center\n \n");
  print_to_list("  Heal Pokémon\n  PC\n  Exit");
  inputNum = get_selection(2, 2, 0);

  clear();
  if (inputNum == 2 || inputNum == PRESSED_B) return POKE_CENTER_EXIT;
  else if (inputNum == 0) {
    player.blackout_center->x = player.loc->x;
    player.blackout_center->y = player.loc->y;
    player.blackout_center->map = player.loc->map;
    heal_party();
    return POKE_CENTER_EXIT;
  }
  else if (inputNum == 1) {
    handle_PC();
    return POKE_CENTER_STAY;
  }

  return POKE_CENTER_EXIT;
}


//Handle all Pokemon Storage movement
void handle_PC() {
  int input_num;
  
  while (1) {
    begin_list();
    print_to_list("Welcome to the PC! Select a Pokemon from storage below");
    print_pokemon_list(player.pc_storage, player.numInPCStorage);
    print_to_list("  Cancel");

    input_num = get_selection(1, player.numInPCStorage, 0);
    if (input_num == player.numInPCStorage || input_num == PRESSED_B) { break; }   //Cancel
    else {
      clear();
      begin_list();
      print_pokemon_summary(&(player.pc_storage[input_num]));
      sleep(2);
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