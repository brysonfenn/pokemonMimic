#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdint.h>

struct Pokemon;
struct Location;
struct Item;

#define PLAYER_DEFAULT_POKEMON -1
#define MAX_IN_PC_STORAGE 18


struct playerCharacter {
  char name[32];

  struct Pokemon * party;
  struct Item * bag;
  int * key_items;
  int * hm_tms;
  struct Pokemon * pc_storage;

  uint8_t numInParty;
  uint8_t numInBag;
  uint8_t numKeyItems;
  uint8_t numHMTMs;
  uint8_t numInPCStorage;

  long long NPCs_done;

  uint8_t trainers_battled_id[256];
  uint16_t num_trainers_battled;
  int32_t money;

  bool trainer_battle;
  bool is_battle;
  
  struct Pokemon * current_pokemon;
  struct Pokemon * enemy_pokemon;

  struct Location * loc;
  struct Location * blackout_center;
};

extern struct playerCharacter player;

//Initialize player struct
//  If save_file == 0, start new game
//  If save_file != 0, load player from previous save file
void player_init(int save_file);

//Return the number of pokemon in the player's party that have not fainted
int player_get_num_alive();

//Set current pokemon according to position in the party
//  if position == PLAYER_DEFAULT_POKEMON, set to first pokemon alive
void set_current_pokemon(int position);

//Set enemy pokemon using a pointer to it
void set_enemy_pokemon(struct Pokemon * pok);

//Print player information
void printPlayer();

//Add a trainer id to list of battled trainer
void add_battled_trainer(int id);

//Returns true only if the player has already battled the trainer
bool has_battled_trainer(int id);

#endif // PLAYER_H