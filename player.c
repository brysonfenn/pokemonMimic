#include "player.h"
#include "stdlib.h"
#include "unistd.h"

struct playerCharacter player;

void default_load();

void player_init() {
  player.bag = malloc(sizeof(item) * 30);
  
  default_load();

  player.current_pokemon = &(player.party[0]);
  player.trainer_battle = false;
}

void default_load() {
  player.numInParty = 2;
  player.numAlive = player.numInParty;
  player.party[0] = charmander;
  player.party[1] = bulbasaur;
  for (int i = 0; i < player.numInParty; i++) {
    randomize_stats(&(player.party[i]), 5, 0, 0);
  }
  
  player.numInBag = 3;
  player.bag[0] = potion;
  player.bag[0].number += 2;
  player.bag[1] = super_potion;
  player.bag[1].number += 2;
  player.bag[2] = pokeball;
  player.bag[2].number += 5;
  player.money = 1000;
}

void printParty() {
  printf("Pokemon:\n");
  for (int i = 0; i < player.numInParty; i++) {
    int current = player.party[i].currentHP;
    int max = player.party[i].maxHP;
    printf("%d: %s\tLVL %d\tHP: %d/%d", i, player.party[i].name, player.party[i].level, current, max);
    if (!(current)) printf("  (Fainted)");
    printf("\n");
  }
}

void printBag() {
  printf("Bag:\n");
  for (int i = 0; i < player.numInBag; i++) {
    printf("%d: %s\t%d\n", i, player.bag[i].name, player.bag[i].number);
  }
  printf("%d: Cancel\n\n", player.numInBag);
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
  }
  player.numAlive = player.numInParty;
  printf("Your Pokemon were restored to full health!\n");
  sleep(2);
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

void printPlayer() {
  printf("B:\n");
  printf("Number of Pokemon: %d\n", player.numInParty);
  printf("Money: $%d\n", player.money);
}