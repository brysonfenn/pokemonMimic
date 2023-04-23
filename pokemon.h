#ifndef POKEMON_H
#define POKEMON_H

#include "attacks.h"
#include "stdbool.h"
#include "stdint.h"
#include "conditions.h"

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

  int numAttacks;

  int16_t atk_stage;
  int16_t def_stage;
  int16_t spd_stage;
  float accuracy;

  uint8_t level;
  uint32_t exp;
  Condition condition;
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


void pokemon_init();

pokemon * get_random_pokemon(int level_min, int level_max);

pokemon * get_random_wild_pokemon(int level_min, int level_max);

void randomize_stats(pokemon * new_pok, int level, int level_min, int level_max);

void reset_base_stats(pokemon *pok);

void print_pokemon_summary(pokemon *pok);

float get_stat_modifier(int16_t stage);

#endif // POKEMON_H