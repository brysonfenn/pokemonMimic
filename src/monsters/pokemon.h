#ifndef POKEMON_H
#define POKEMON_H

#include <stdint.h>

#include "attacks.h"
#include "conditions.h"
#include "typings.h"

#define NUM_CREATED_POKEMON 8
#define RANDOM_LEVEL 0

typedef struct pokemon {
  char name[30];
  int id_num;
  int maxHP;
  int currentHP;
  
  int16_t baseAttack;
  int16_t baseDefense;
  int16_t baseSpeed;

  Type type1;
  Type type2;

  int numAttacks;
  int16_t atk_stage;
  int16_t def_stage;
  int16_t spd_stage;
  uint8_t level;
  uint8_t sleep_count;
  uint32_t exp;
  float accuracy;

  Condition visible_condition;
  Condition hidden_condition;

  attack attacks[4];
} pokemon;

extern pokemon emptyPok;

extern pokemon bulbasaur;
extern pokemon charmander;
extern pokemon squirtle;

extern pokemon caterpie;
extern pokemon weedle;
extern pokemon pidgey;
extern pokemon rattata;
extern pokemon sandshrew;

//pokemon.c

//Initialize a given pokemon new_pok and get randomized stats
  //level = particular level
  //if level = 0, get random level from range level_min:level_max
void pokemon_init(pokemon * new_pok, int level, int level_min, int level_max);

//Reset stat stages whenever pokemon is switched out or battle ends
void reset_base_stats(pokemon *pok);

//Print stats, attacks, etc of a given pokemon
void print_pokemon_summary(pokemon *pok);

//Return the fraction modifier for a stat given the stage (stage 0 return 1.0)
float get_stat_modifier(int16_t stage);

//Handle leveling up - also handles learning moves from new level
void pokemon_level_up(pokemon *pok, int next_level_exp);

//Give highest possible level moves to a pokemon
//This is called upon initializing a new pokemon
void pokemon_give_moves(pokemon *pok);

//Give random stats to pokemon based on level
  //level = particular level
  //if level = 0, get random level from range level_min:level_max
void randomize_stats(pokemon * new_pok, int level, int level_min, int level_max);



//pokemon_list.c

//Return a random pokemon of any possible
pokemon * get_random_pokemon(int level_min, int level_max);

//Return a random pokemon, excluding starters
//Always immediately dereference the return value of this function.
pokemon * get_random_wild_pokemon(int level_min, int level_max);

#endif // POKEMON_H