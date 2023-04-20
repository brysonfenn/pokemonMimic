#ifndef PLAYER_H
#define PLAYER_H

#include "items.h"
#include "pokemon.h"
#include "stdbool.h"

#define PLAYER_DEFAULT_POKEMON -1


struct playerCharacter {
  pokemon party[6];
  int numInParty;
  int numAlive;
  struct item * bag;
  int numInBag;
  pokemon * current_pokemon;
  int money;
  bool trainer_battle;
};

extern struct playerCharacter player;

void player_init();

void printParty();

void printBag();

bool runAttempt();

void set_current_pokemon(int position);

void printPlayer();

#endif // PLAYER_H