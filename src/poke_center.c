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
void handle_poke_center() {
  int inputNum;
  begin_list();
  print_to_list("Welcome to the Pokémon Center\n \n");
  print_to_list("  Heal Pokémon\n  PC\n  Exit");
  inputNum = get_selection(LIST_BOX_Y+3, 2, 0);

  clear();
  if (inputNum == 2 || inputNum == PRESSED_B) return;
  else if (inputNum == 0) {
    player.blackout_center->x = player.loc->x;
    player.blackout_center->y = player.loc->y;
    player.blackout_center->map = player.loc->map;
    heal_party();
  }
  else if (inputNum == 1) {
    handle_PC();
    sleep(2);
  }
}


//Handle all Pokemon Storage movement
void handle_PC() {
    begin_list();
    print_to_list("Welcome to the PC!");
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