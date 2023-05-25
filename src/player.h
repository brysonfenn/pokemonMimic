#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

struct Pokemon;
struct Location;
struct Item;

#define PLAYER_DEFAULT_POKEMON -1


struct playerCharacter {
  struct Pokemon * party;
  int numInParty;
  struct Item * bag;
  int numInBag;
  struct Pokemon * current_pokemon;
  int money;
  bool trainer_battle;
  struct Pokemon * enemy_pokemon;
  struct Location * loc;
  struct Location * blackout_center;
};

extern struct playerCharacter player;

void player_init(int save_file);

bool runAttempt();

void handle_poke_center();

int player_get_num_alive();

void set_current_pokemon(int position);

void set_enemy_pokemon(struct Pokemon * pok);

void printPlayer();

#endif // PLAYER_H