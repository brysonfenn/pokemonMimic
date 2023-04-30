#include "player.h"

#include "stdlib.h"
#include "unistd.h"

#include "conditions.h"
#include "maps/location.h"
#include "maps/map_drawing.h"

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

void printParty() {
  printf("Pokemon:\n");
  for (int i = 0; i < player.numInParty; i++) {
    pokemon current_pok = player.party[i];
    int current = current_pok.currentHP;
    int max = current_pok.maxHP;
    printf("%d: %s\tLVL %d\tHP: %d/%d  ", i+1, current_pok.name, current_pok.level, current, max);
    if (!(current)) printf(" (Fainted) ");
    print_condition(&current_pok);
    printf("\n");
  }
}

void printBag() {
  printf("Bag:\n");
  for (int i = 0; i < player.numInBag; i++) {
    printf("%d: %s\t%d\n", i+1, player.bag[i].name, player.bag[i].number);
  }
  printf("0: Cancel\n\n");
}

bool runAttempt() {
  int success = ((rand() % 4) > 0);
  if (success) {
    printf("Got away safely.\n");
  }
  else {
    printf("Can't escape!\n");
  }
  sleep(2);

  return success;
}

void heal_party() {
  for (int i = 0; i < player.numInParty; i++) {
    player.party[i].currentHP = player.party[i].maxHP;
    player.party[i].visible_condition = NO_CONDITION;
  }
  player.numAlive = player.numInParty;
  printf("Your Pokemon were restored to full health!\n");
  sleep(2);
}

void handle_poke_center() {
  int inputNum;
  printf("Welcome to the Pokémon Center\n\n");
  printf("1: Heal Pokémon\n0: Exit\n\n");
  inputNum = getValidInput(0, 1, "Select an Option: ");
  if (!inputNum) return;
  else if (inputNum == 1) {
    heal_party(); clearTerminal();
    printParty(); sleep(2); clearTerminal();
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
  printf("B:\n");
  printf("Number of Pokemon: %d\n", player.numInParty);
  printf("Money: $%d\n", player.money);
}