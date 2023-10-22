#ifndef ATTACKS_H
#define ATTACKS_H

#include <stdint.h>
#include <stdbool.h>

#include "conditions.h"
#include "typings.h"

#define NO_MISS 1000

//Percentage of user's own HP
#define HP_PERCENTAGE 1
//Specific amount of HP
#define HP_AMOUNT 2
//Percentage of damage dealt by user
#define PERCENT_DAMAGE_DEALT 3

#define STRUGGLE_MOVE_NUM -1

#define ATTACK_SUCCESS 0
#define ATTACK_FAIL 1
#define ATTACK_END_BATTLE 2
#define ATTACK_SPECIAL_MOVE -102


struct Pokemon;

typedef int (*attack_func_ptr) (Condition, int, struct Pokemon*, int);

typedef struct attack {
  char name[30];
  int id_num;
  int max_pp;
  int power; 
  int accuracy;
  Type type;
  
  bool priority;
  attack_func_ptr side_effect;
  int var1;
  int var2;

  int curr_pp;
} attack;

extern attack empty_attack;



//// attacks.c ////

//Handle all operations for an attack, indicate whether this is an enemy attack (true)
int perform_attack(struct Pokemon *perp, int move_num, struct Pokemon *victim, bool enemy);

//Get damage that should be dealt with a given move from one pokemon to another
int get_damage(struct Pokemon *perp, int move_num, struct Pokemon *victim, bool print_statements, int *flags);

//Basic Damage function
int get_basic_damage(int perp_level, int attack_power, int perp_atk, int victim_def, int perp_atk_stage, int victim_def_stage);


//// attacks_list.c ////

//Return an attack given an attack id number
attack * get_attack_by_id(int id_num);

//Filler side-effect function that does nothing
int attack_do_nothing(Condition condition, int accuracy, struct Pokemon * pok, int damage);

//Certain attacks can increment a pokemon's own stat
int increment_self_stat(Condition stat_type, int chance, struct Pokemon* victim, int damage);

int increment_self_stat2(Condition stat_type, int chance, struct Pokemon* victim, int damage);

//Certain attacks can decrement an enemy pokemon's stat
int decrement_opponent_stat(Condition stat_type, int chance, struct Pokemon* victim, int damage);

//Certain attacks can double decrement an enemy pokemon's stat
int decrement_opponent_stat2(Condition stat_type, int chance, struct Pokemon* victim, int damage);

//Some attacks do a specific amount of damage
int deal_specific_damage(Condition nothing1, int hp, struct Pokemon* victim, int nothing2);

//Some attacks deal a percentage of damage left
int deal_percentage_damage(Condition nothing, int percent, struct Pokemon* victim, int damage);

//Some attacks hit multiple times
int hit_2_5_times(int nothing1, int nothing2, struct Pokemon* victim, int damage);

//Some attacks hit twice
int hit_twice(int nothing1, int nothing2, struct Pokemon* victim, int damage);

//Heal a specified amount of HP
int self_heal(int heal_type, int hp, struct Pokemon* victim, int damage);

//Inflict damage to self
int self_inflict_damage(int damage_type, int hp, struct Pokemon* victim, int damage);

#endif // ATTACKS_H