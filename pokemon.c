#include "pokemon.h"
#include "stdio.h"
#include "unistd.h"
#include "stdbool.h"
#include "stdlib.h"

// pok  varName   = {"name here",  id  hp chp   ba   bd    bs  num_att ca cd cs  acc lvl exp}
pokemon emptyPok = {"MissingNo",    0,100,100,   0,   0,    0 };
pokemon bulbasaur = {"Bulbasaur",   1,  9,  9,   4,   4,    4 };
pokemon charmander = {"Charmander", 4,  9,  9,   4,   4,    4 };
pokemon squirtle = {"Squirtle",     7,  9,  9,   4,   4,    4 };
pokemon caterpie = {"Caterpie",    10,  6,  6,   2,   2,    3 };
pokemon weedle   = {"Weedle",      13,  7,  7,   3,   2,    3 };
pokemon pidgey   = {"Pidgey",      16,  8,  8,   4,   2,    3 };
pokemon rattata  = {"Rattata",     19,  7,  7,   2,   3,    4 };
pokemon sandshrew= {"Sandshrew",   27,  7,  7,   3,   4,    2 };

#define NUM_CREATED_POKEMON 8
#define NUM_STARTERS 3
#define NUM_WILD_POKEMON (NUM_CREATED_POKEMON - NUM_STARTERS)
static pokemon * pokList[10] = { &bulbasaur, &charmander, &squirtle, &caterpie, &weedle, &pidgey, &rattata, &sandshrew };

static pokemon newest_pokemon;

void pokemon_set_attacks(pokemon * pok, int numAttacks, attack att1,attack att2,attack att3,attack att4) {
  pok->condition = NO_CONDITION;
  pok->numAttacks = numAttacks;
  pok->attacks[0] = att1;
  pok->attacks[1] = att2;
  pok->attacks[2] = att3;
  pok->attacks[3] = att4;
}

void pokemon_init() {
  //                    Name        numattacks
  pokemon_set_attacks(&bulbasaur,   2, tackle,         growl,          empty_attack,   empty_attack);
  pokemon_set_attacks(&charmander,  2, scratch,        tail_whip,      empty_attack,   empty_attack);
  pokemon_set_attacks(&squirtle,    2, tackle,         tail_whip,      empty_attack,   empty_attack);
  pokemon_set_attacks(&caterpie,    2, tackle,         string_shot,    empty_attack,   empty_attack);
  pokemon_set_attacks(&weedle,      1, poison_sting,   empty_attack,   empty_attack,   empty_attack);
  pokemon_set_attacks(&pidgey,      2, tackle,         sand_attack,    empty_attack,   empty_attack);
  pokemon_set_attacks(&rattata,     2, tackle,         quick_attack,   empty_attack,   empty_attack);
  pokemon_set_attacks(&sandshrew,   2, scratch,         sand_attack,    empty_attack,   empty_attack);
}

pokemon * get_random_pokemon(int level_min, int level_max) {
  newest_pokemon = *(pokList[rand() %  NUM_CREATED_POKEMON]);
  pokemon * new_pok = &newest_pokemon;
  randomize_stats(new_pok, RANDOM_LEVEL, level_min, level_max);
  return new_pok;
}

//Always immediately dereference the return value of this function.
pokemon * get_random_wild_pokemon(int level_min, int level_max) {
  newest_pokemon = *(pokList[(rand() % NUM_WILD_POKEMON) + NUM_STARTERS]);
  // newest_pokemon = caterpie;
  pokemon * new_pok = &newest_pokemon;
  randomize_stats(new_pok, RANDOM_LEVEL, level_min, level_max);
  return new_pok;
}

void randomize_stats(pokemon * new_pok, int level, int level_min, int level_max) {
  if (!level) {
    level = (rand() % (level_max - level_min)) + level_min;
  }
  new_pok->level = level;
  new_pok->exp = 0;
  new_pok->maxHP +=  (2*level) +  (rand() % 3);
  new_pok->currentHP = new_pok->maxHP;
  new_pok->baseAttack += (level + 2) + (rand() % 3);
  new_pok->baseDefense += (level + 3) + (rand() % 2);
  new_pok->baseSpeed += (level + 2) + (rand() % 5);
  reset_base_stats(new_pok);
}

void reset_base_stats(pokemon *pok) {
  pok->atk_stage = 0;
  pok->def_stage = 0;
  pok->spd_stage = 0;
  pok->accuracy  = 1.0;
}

void print_pokemon_summary(pokemon *pok) {
  printf("%s  LVL %d:\n", pok->name, pok->level);
  printf("EXP to next Level: %d\n\n", (pok->level * 8) - pok->exp);
  printf("HP: %d/%d", pok->currentHP, pok->maxHP);
  if (!(pok->currentHP)) printf("  (Fainted)");
  printf("\nBase Attack: %d\nBase Defense: %d\n", pok->baseAttack, pok->baseDefense);
  printf("Base Speed: %d\n\n", pok->baseSpeed);
  printf("Attacks: \n");
  printf("\t%s\t%s\n\t%s\t%s\n", pok->attacks[0].name, pok->attacks[1].name, pok->attacks[2].name, pok->attacks[3].name);
}

float get_stat_modifier(int16_t stage) {
  float modified_stat = 1.0;
  if (stage >= 0) { modified_stat = (2.0 + stage) / 2.0; }  // positive means 3/2, 4/2, etc.
  if (stage < 0) { modified_stat = 2.0 / (2.0 - stage); }   // negative means 2/3, 2/4, etc.
  return modified_stat;
}