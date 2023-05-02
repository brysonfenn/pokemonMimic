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


void pokemon_init(pokemon * new_pok, int level, int level_min, int level_max);

pokemon * get_random_pokemon(int level_min, int level_max);

pokemon * get_random_wild_pokemon(int level_min, int level_max);

void reset_base_stats(pokemon *pok);

void print_pokemon_summary(pokemon *pok);

float get_stat_modifier(int16_t stage);

void pokemon_level_up(pokemon *pok, int next_level_exp);

void pokemon_give_moves(pokemon *pok);

#endif // POKEMON_H