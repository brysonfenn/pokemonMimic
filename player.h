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
  pokemon * enemy_pokemon;
};

extern struct playerCharacter player;

void player_init(int save_file);

void printParty();

void printBag();

bool runAttempt();

void handle_poke_center();

void set_current_pokemon(int position);

void set_enemy_pokemon(pokemon * pok);

void printPlayer();

#endif // PLAYER_H