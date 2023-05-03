#include "player.h"

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "monsters/conditions.h"
#include "maps/location.h"
#include "maps/map_drawing.h"
#include "print_utils.h"

struct playerCharacter player;

void default_load();

void player_init(int save_file) {
  player.bag = malloc(sizeof(item) * 30);
  
  if (!save_file)
    default_load();
  else 
    load_game(save_file);

  player.current_pokemon = &(player.party[0]);
  player.trainer_battle = false;
}

void default_load() {
  player.numInParty = 2;
  player.numAlive = player.numInParty;
  player.party[0] = charmander;
  player.party[1] = bulbasaur;
  for (int i = 0; i < player.numInParty; i++) {
    pokemon_init(&(player.party[i]), 5, 0, 0);
  }
  
  player.numInBag = 3;
  player.bag[0] = potion;
  player.bag[0].number += 2;
  player.bag[1] = super_potion;
  player.bag[1].number += 2;
  player.bag[2] = pokeball;
  player.bag[2].number += 5;
  player.money = 1000;

  player.loc = malloc(sizeof(Location));

  player.loc->x = MAP_X + 10;
  player.loc->y = MAP_Y + 10;
}

bool runAttempt() {
  int success = ((rand() % 4) > 0);
  clear_text_box();
  
  text_box_cursors(0);
  if (success) {
    printw("Got away safely.");
  }
  else {
    printw("Can't escape!");
  }
  refresh();
  sleep(2);

  return success;
}

void heal_party() {
  for (int i = 0; i < player.numInParty; i++) {
    player.party[i].currentHP = player.party[i].maxHP;
    player.party[i].visible_condition = NO_CONDITION;
  }
  player.numAlive = player.numInParty;
  printw("Your Pokemon were restored to full health!\n"); refresh();
  sleep(2);
}

void handle_poke_center() {
  int inputNum;
  clear();
  printw("Welcome to the Pokémon Center\n\n");
  printw("  Heal Pokémon\n  Exit\n\n");
  // inputNum = getValidInput(0, 1, "Select an Option: ");
  inputNum = get_selection(2,0,1,0);

  clear();
  if (inputNum == 1) return;
  else if (inputNum == 0) {
    heal_party(); clear();
    printParty(); sleep(2); clear();
  }
}

void set_current_pokemon(int position) {
  if (position < 0 || position >= player.numInParty || player.party[position].currentHP == 0) {
    // Find first available alive pokemon
    for (int i = 0; i < player.numInParty; i++) {
      if (player.party[i].currentHP > 0) {
        player.current_pokemon = &(player.party[i]);
        return;
      }
    }
  }
  else {
    player.current_pokemon = &(player.party[position]);
    return;
  }
  player.current_pokemon = &(emptyPok);
}

void set_enemy_pokemon(pokemon * pok) {
  player.enemy_pokemon = pok;
}

void printPlayer() {
  move(0,0);
  printw("  B:\n");
  printw("  Number of Pokemon: %d\n", player.numInParty);
  printw("  Money: $%d\n", player.money);
  refresh();
  sleep(2);
  
}