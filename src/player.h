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
  struct item * bag;
  int numInBag;
  struct pokemon * current_pokemon;
  int money;
  bool trainer_battle;
  struct pokemon * enemy_pokemon;
  struct Location * loc;
  struct Location * blackout_center;
};

extern struct playerCharacter player;

void player_init(int save_file);

bool runAttempt();

void handle_poke_center();

int player_get_num_alive();

void set_current_pokemon(int position);

void set_enemy_pokemon(struct pokemon * pok);

void printPlayer();

#endif // PLAYER_H