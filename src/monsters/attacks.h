#ifndef ATTACKS_H
#define ATTACKS_H

#include <stdint.h>
#include <stdbool.h>

#include "conditions.h"
#include "typings.h"

#define NO_MISS 1000

struct pokemon;

typedef int (*attack_func_ptr) (Condition, int, struct pokemon*, int);

typedef struct attack {
  char name[30];
  int id_num;
  int max_pp;
  int power; 
  int accuracy;
  Type type;
  
  bool priority;
  attack_func_ptr side_effect;
  int condition;
  int chance;

  int curr_pp;
} attack;

extern attack empty_attack;

//// attacks.c ////

//Handle all operations for an attack, indicate whether this is an enemy attack (true)
int perform_attack(struct pokemon *perp, int move_num, struct pokemon *victim, bool enemy);

//Get damage that should be dealt with a given move from one pokemon to another
int getDamage(struct pokemon *perp, int move_num, struct pokemon *victim, bool print_statements);

//// attacks_list.c ////

//Return an attack given an attack id number
attack * get_attack_by_id(int id_num);

//Certain attacks can increment a pokemon's own stat
int increment_self_stat(Condition stat_type, int chance, struct pokemon* victim, int damage);

int increment_self_stat2(Condition stat_type, int chance, struct pokemon* victim, int damage);

//Certain attacks can decrement an enemy pokemon's stat
int decrement_opponent_stat(Condition stat_type, int chance, struct pokemon* victim, int damage);

//Certain attacks can double decrement an enemy pokemon's stat
int decrement_opponent_stat2(Condition stat_type, int chance, struct pokemon* victim, int damage);

//Some attacks do a specific amount of damage
int deal_specific_damage(Condition nothing, int hp, struct pokemon* victim, int damage);

//Some attacks hit multiple times
int hit_multiple_times(int min_times, int max_times, struct pokemon* victim, int damage);

#endif // ATTACKS_H