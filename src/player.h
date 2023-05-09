#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

struct pokemon;
struct Location;
struct item;

#define PLAYER_DEFAULT_POKEMON -1


struct playerCharacter {
  struct pokemon * party;
  int numInParty;
  int numAlive;
  struct item * bag;
  int numInBag;
  struct pokemon * current_pokemon;
  int money;
  bool trainer_battle;
  struct pokemon * enemy_pokemon;
  struct Location * loc;
};

extern struct playerCharacter player;

void player_init(int save_file);

bool runAttempt();

void handle_poke_center();

void set_current_pokemon(int position);

void set_enemy_pokemon(struct pokemon * pok);

void printPlayer();

#endif // PLAYER_H