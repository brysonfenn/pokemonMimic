#include "player.h"

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "monsters/conditions.h"
#include "monsters/pokemon.h"
#include "motion/location.h"
#include "motion/map_drawing.h"
#include "print/print_utils.h"
#include "print/print_defines.h"
#include "print/print_battle.h"
#include "items/items.h"
#include "items/key_items.h"

struct playerCharacter player;

void default_load();

//Initialize player struct
//  If save_file == 0, start new game
//  If save_file != 0, load player from previous save file
void player_init(int save_file) { 
  player.bag = malloc(sizeof(Item) * 30);
  player.key_items = malloc(sizeof(int) * 30);
  player.hm_tms = malloc(sizeof(int) * 30);
  
  player.loc = malloc(sizeof(Location));
  player.blackout_center = malloc(sizeof(Location));

  player.party = malloc(sizeof(Pokemon) * 6);
  player.pc_storage = malloc(sizeof(Pokemon) * 20);

  player.num_trainers_battled = 0;
  
  if (!save_file)
    default_load();
  else 
    load_game(save_file);

  player.current_pokemon = &(player.party[0]);
  player.trainer_battle = false;
}


//Start player at new game
void default_load() {

  sprintf(player.name, "Player");
  
  player.numInBag = 2;
  player.bag[0] = *get_item_by_id(POTION);
  player.bag[0].number += 2;
  player.bag[1] = *get_item_by_id(POKE_BALL);
  player.bag[1].number += 5;
  player.money = 1000;
  player.NPCs_done = 0;

  player.numKeyItems = 0;
  player.numHMTMs = 0;
  player.numInPCStorage = 0;

  player.loc->x = MAP_X + 10;
  player.loc->y = MAP_Y + 10;
  player.loc->map = 4;

  player.blackout_center->x = MAP_X + 10;
  player.blackout_center->y = MAP_Y + 10;
  player.blackout_center->map = 4;
}


//Return the number of pokemon in the player's party that have not fainted
int player_get_num_alive() {
  int numAlive = 0;
  for (int i = 0; i < player.numInParty; i++) {
    if (player.party[i].currentHP > 0) numAlive++;
  }
  return numAlive;
}


//Set current pokemon according to position in the party
//  if position == PLAYER_DEFAULT_POKEMON, set to first pokemon alive
void set_current_pokemon(int position) {
  if (position < 0 || position >= player.numInParty || player.party[position].currentHP == 0) {
    // Find first available alive pokemon
    for (int i = 0; i < player.numInParty; i++) {
      if (player.party[i].currentHP > 0) {
        player.current_pokemon = &(player.party[i]);
        reset_stat_stages(player.current_pokemon);
        return;
      }
    }
  }
  else {
    player.current_pokemon = &(player.party[position]);
    reset_stat_stages(player.current_pokemon);
    return;
  }
  player.current_pokemon = &(emptyPok);
}


//Set enemy pokemon using a pointer to it
void set_enemy_pokemon(struct Pokemon * pok) {
  player.enemy_pokemon = pok;
}


//Print player information
void printPlayer() {
  char print_str[1024] = "";
  sprintf(print_str, "%s  %s:\n", print_str, player.name);
  sprintf(print_str, "%s  Number of Pokemon: %d\n", print_str, player.numInParty);
  sprintf(print_str, "%s  Money: $%d\n \n  Badges:\n    ", print_str, player.money);

  for (int leader_id = 201; leader_id <= 208; leader_id++) {
    if (has_battled_trainer(leader_id))
      sprintf(print_str, "%s%d  ", print_str, leader_id - 200);
    else
      sprintf(print_str, "%s-  ", print_str);

    if (leader_id == 204)
      sprintf(print_str, "%s\n    ", print_str);
  }

  print_to_list(print_str);
  refresh();
}


//Handle trainers already battled

//Add a trainer id to list of battled trainer
void add_battled_trainer(int id) {
  player.trainers_battled_id[player.num_trainers_battled] = id;
  player.num_trainers_battled++;
}

//Returns true only if the player has already battled the trainer
bool has_battled_trainer(int id) {
  for (int i = 0; i < player.num_trainers_battled; i++) {
    if (player.trainers_battled_id[i] == id) return true;
  }
  return false;
}