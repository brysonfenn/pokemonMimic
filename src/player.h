#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdint.h>

struct Pokemon;
struct Location;
struct Item;

#define PLAYER_DEFAULT_POKEMON -1


struct playerCharacter {
  struct Pokemon * party;
  struct Item * bag;
  struct Pokemon * storage;

  uint8_t numInParty;
  uint8_t numInBag;
  uint8_t numInStorage;

  uint8_t trainers_battled_id[256];
  uint16_t num_trainers_battled;
  int32_t money;

  bool trainer_battle;
  
  struct Pokemon * current_pokemon;
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

void add_battled_trainer(int id);

bool trainer_already_battled(int id);

#endif // PLAYER_H