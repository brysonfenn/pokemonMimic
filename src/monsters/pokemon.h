#ifndef POKEMON_H
#define POKEMON_H

#include <stdint.h>

#include "conditions.h"
#include "typings.h"
#include "pokemon_id_list.h"
#include "attacks.h"

#define RANDOM_LEVEL 0

#define NO_LAST_MOVE -1

#define IV_HP 0
#define IV_ATTACK 1
#define IV_DEFENSE 2
#define IV_SP_ATTACK 3
#define IV_SP_DEFENSE 4
#define IV_SPEED 5

#define RETURN_TO_SUMMARY 0
#define RETURN_TO_MENU 1
#define RETURN_TO_PARTY 2

typedef struct Pokemon {
  char name[30];
  char nickname[16];
  int16_t id_num;
  int16_t maxHP;
  int16_t currentHP;
  
  int16_t baseAttack;
  int16_t baseDefense;
  int16_t baseSpAttack;
  int16_t baseSpDefense;
  int16_t baseSpeed;

  int32_t iv;
  int32_t ev;

  Type type1;
  Type type2;

  int8_t numAttacks;

  int8_t atk_stage;
  int8_t def_stage;
  int8_t sp_atk_stage;
  int8_t sp_def_stage;
  int8_t spd_stage;
  int8_t acc_stage;
  int8_t evade_stage;

  int8_t level;
  int8_t sleep_count;
  uint32_t exp;
  
  Condition visible_condition;
  Condition * hidden_conditions;
  int8_t * hidden_condition_values;
  int16_t num_hidden_conditions;
  
  int8_t last_move;

  attack attacks[4];
} Pokemon;

extern Pokemon emptyPok;


//pokemon.c

//Reset stat stages whenever pokemon is switched out or battle ends
void reset_stat_stages(Pokemon *pok);

//Print stats, attacks, etc of a given pokemon
void print_pokemon_summary(Pokemon *pok);

//Control all actions for the pokemon menu
int handle_pokemon_menu(int input_num1);

//Return the fraction modifier for a stat given the stage (stage 0 return 1.0)
float get_stat_modifier(int16_t stage);

//Give random stats to pokemon based on level
  //level = particular level
  //if level = 0, get random level from range level_min:level_max
void calculate_stats(Pokemon * pok, int level, int level_min, int level_max);

//Get the catch rate for a pokemon with pok_id
int pokemon_get_catch_rate(int pok_id);

int pokemon_get_accuracy(Pokemon * perp, int move_num, Pokemon * victim);



//pokemon_progression.c

//Handle leveling up - also handles learning moves from new level
void pokemon_level_up(Pokemon *pok, int next_level_exp);

//Return exp required for the next level
uint32_t pokemon_get_next_level_exp(Pokemon *pok);

//Return exp yield of a defeated pokemon
uint32_t pokemon_get_exp_yield(Pokemon *pok);

//Give highest possible level moves to a pokemon
//This is called upon initializing a new pokemon
void pokemon_give_moves(Pokemon *pok);

//Return iv field
int pokemon_get_iv(Pokemon * pok, char type);

//Return ev field
int pokemon_get_ev(Pokemon * pok, char type);

//Increment ev for a pokemon
int pokemon_increment_ev(Pokemon * pok, char type);

//Put evolution level for pokemon with ID # pok_id in level_target and ID # of evolution in evolve_id
void get_evolve_level_and_id(int pok_id, int * level_target, int * evolve_id);

void learn_move(Pokemon * pok, attack * new_attack);

void handle_evolve_outside_battle(Pokemon * pok, int next_pok_id);



//pokemon_list.c

//Create a new pokemon. **Immediately dereference the returned pokemon**
Pokemon * create_new_pokemon(Pokemon_id pok_id, int level, int level_min, int level_max);

//Free all memory used by a pokemon
void destroy_pokemon(Pokemon * pok);

//Return a random pokemon, excluding starters
//Always immediately dereference the return value of this function.
Pokemon * get_random_wild_pokemon();

//Get a starter pokemon
Pokemon * get_starter(int starter);

//Get a pokemon with all its base stats
Pokemon * get_pokemon_frame(Pokemon_id pok_id);

//Add Pokemon to player party or PC (returns false if not possible)
bool give_pokemon_to_player(Pokemon * pok);

#endif // POKEMON_H