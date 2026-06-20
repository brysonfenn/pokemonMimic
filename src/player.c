#include "player.h"

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "monsters/conditions.h"
#include "monsters/pokemon.h"
#include "motion/location.h"
#include "motion/map_drawing.h"
#include "motion/maps.h"
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
    player.pc_storage = malloc(sizeof(Pokemon) * 150);

    player.num_trainers_battled = 0;
    player.num_flyable_cities = 0;
    player.player_char = PLAYER_MOVING_DOWN;
    player.original_starter = POKEMON_BULBASAUR;
    player.safari_balls = 0;
    player.bait_count = 0;
    sprintf(player.rival_name, "Not Found");
    
    if (!save_file)
        default_load();
    else 
        load_game(save_file);

    player.current_pokemon = &(player.party[0]);
    player.is_trainer_battle = false;
    player.is_uncaught_pokemon = false;
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
    player.record_bits = 0;
    player.repel_steps = 0;

    player.numKeyItems = 0;
    player.numHMTMs = 0;
    player.numInPCStorage = 0;

    player.loc->x = MAP_X + 10;
    player.loc->y = MAP_Y + 5;
    player.loc->map = 4;

    player.blackout_center->x = MAP_X + 10;
    player.blackout_center->y = MAP_Y + 5;
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
void player_set_current_pokemon(int position) {
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
void player_set_enemy_pokemon(struct Pokemon * pok) {
    player.enemy_pokemon = pok;
}


//Print player information
void player_print() {
    char print_str[1024] = "";
    sprintf(print_str, "%s  %s:\n", print_str, player.name);
    sprintf(print_str, "%s  Number of Pokemon: %d\n", print_str, player.numInParty + player.numInPCStorage);
    sprintf(print_str, "%s  Money: $%d\n \n  Badges:\n    ", print_str, player.money);

    for (int leader_id = 201; leader_id <= 208; leader_id++) {
        if (player_has_battled_trainer(leader_id)) sprintf(print_str, "%s%d  ", print_str, leader_id - 200);
        else sprintf(print_str, "%s-  ", print_str);

        if (leader_id == 204) sprintf(print_str, "%s\n    ", print_str);
    }

    print_to_list(print_str);
    refresh();
}


//Handle trainers already battled

//Add a trainer id to list of battled trainer
void player_add_battled_trainer(int id) {
    player.trainers_battled_id[player.num_trainers_battled] = id;
    player.num_trainers_battled++;
}

//Returns true only if the player has already battled the trainer
bool player_has_battled_trainer(int id) {
    for (int i = 0; i < player.num_trainers_battled; i++) {
        if (player.trainers_battled_id[i] == id) return true;
    }
    return false;
}

//Returns true only if the player has the pokemon in PC or Party
bool player_has_pokemon(int id) {
    for (int i = 0; i < player.numInParty; i++) {
        if (player.party[i].id_num == id) return true;
    }
    for (int i = 0; i < player.numInPCStorage; i++) {
        if (player.pc_storage[i].id_num == id) return true;
    }

    return false;
}


// Returns -1 if key_item not in list, else returns the index of the key item
int player_get_key_item_index(int id) {
    for (int i = 0; i < player.numKeyItems; i++) {
        if (player.key_items[i] == id) {
            return i;
        }
    }
    return -1;
}

//Returns false if bit was already set to 1, returns true if bit was set from 0 to 1
bool player_set_record_bit(record_bit_int record_bit_num) {
    if ((player.record_bits >> record_bit_num) & 1) {
        return false;
    }
    else {
        player.record_bits |= ((long long) 1 << record_bit_num);
        return true;
    }
}

//Switch Record bit from 1 to 0 or from 0 to 1: Returns false if bit was set to 0, returns true if bit was set to 1;
bool player_switch_record_bit(record_bit_int record_bit_num) {
    long long mask = (long long) 1 << record_bit_num;

    player.record_bits ^= mask;
    
    if ((player.record_bits >> record_bit_num) & 1) {
        return true;
    }
    else {
        return false;
    }
}

//Returns false if Record bit is set to 0, returns true if bit is set to 1;
bool player_record_bit_is_set(record_bit_int record_bit_num) {
    if ((player.record_bits >> record_bit_num) & 1) {
        return true;
    }
    else {
        return false;
    }
}

//Returns true if city id is flyable
bool player_is_flyable_city(int id) {
    for (int i = 0; i < player.num_flyable_cities; i++) {
        if (player.flyable_cities[i] == id)
            return true;
    }
    return false;
}

//Returns false if city id was already flyable
bool player_add_flyable_city(int id) {
    if (player_is_flyable_city(id)) return false;
    
    player.flyable_cities[player.num_flyable_cities] = id;
    player.num_flyable_cities++;
    return true;
}

//Returns true only if player is inside
bool player_is_inside() {
    int map = player.loc->map;
    
    if (
        map == MAP_LAB || map == MAP_GYM1 || map == MAP_MT_MOON_N || map == MAP_GYM2 || 
        map == MAP_UNDERGROUND || map == MAP_SS_ANNE1 || map == MAP_SS_ANNE2 ||
        map == MAP_GYM3 || map == MAP_MT_MOON_S || map == MAP_DIG_CAVE || 
        map == MAP_MUSEUM || map == MAP_ROCK_TUNNEL_N || map == MAP_ROCK_TUNNEL_S ||
        map == MAP_ROCKET1 || map == MAP_ROCKET2 || map == MAP_ROCKET3 ||
        map == MAP_GAME_CORNER  || map == MAP_GYM4 || map == MAP_TOWER1 ||
        map == MAP_TOWER2 || map == MAP_TOWER3 || map == MAP_GYM5 || map == MAP_GYM6 ||
        map == MAP_GYM7 || map == MAP_GYM8 || map == MAP_SILPH1 || map == MAP_SILPH2 ||
        map == MAP_SILPH3 || map == MAP_SILPH4 || map == MAP_SILPH5 || map == MAP_SILPH6 ||
        map == MAP_MANSION1 || map == MAP_MANSION2 || map == MAP_MANSION3 || 
        map == MAP_MANSION4 
    ) {
        return true;
    }
    else {
        return false;
    }
}