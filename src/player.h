#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdint.h>

struct Pokemon;
struct Location;
struct Item;

#define PLAYER_DEFAULT_POKEMON -1
#define MAX_IN_PC_STORAGE 145

typedef enum { RECORD_BIT_0, RECORD_BIT_1, RECORD_BIT_2, RECORD_BIT_3, RECORD_BIT_4, RECORD_BIT_5, RECORD_BIT_6, RECORD_BIT_7, 
                RECORD_BIT_8, RECORD_BIT_9, RECORD_BIT_10, RECORD_BIT_11, RECORD_BIT_12, RECORD_BIT_13, RECORD_BIT_14, RECORD_BIT_15, 
                RECORD_BIT_16, RECORD_BIT_17
} record_bit_int;


struct playerCharacter {
  char name[32];
  char rival_name[32];

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

  char player_char;
  int original_starter;

  long long record_bits;

  unsigned char trainers_battled_id[256];
  int num_trainers_battled;
  int money;
  int repel_steps;
  int safari_balls;
  int bait_count;

  bool is_trainer_battle;
  bool is_battle;
  bool is_uncaught_pokemon;
  
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
void player_set_current_pokemon(int position);

//Set enemy pokemon using a pointer to it
void player_set_enemy_pokemon(struct Pokemon * pok);

//Print player information
void player_print();

//Add a trainer id to list of battled trainer
void player_add_battled_trainer(int id);

//Returns true only if the player has already battled the trainer
bool player_has_battled_trainer(int id);

//Returns true only if the player has the pokemon in PC or Party
bool player_has_pokemon(int id);

// Returns -1 if key_item not in list, else returns the index of the key item
int player_get_key_item_index(int id);

#endif // PLAYER_H