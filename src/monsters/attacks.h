#ifndef ATTACKS_H
#define ATTACKS_H

#include <stdint.h>
#include <stdbool.h>

#include "conditions.h"
#include "typings.h"

struct pokemon;

typedef int (*attack_func_ptr) (Condition, int, struct pokemon*);

typedef struct attack {
  char name[30];
  int id_num;
  int pp;
  int power; 
  uint8_t accuracy;
  Type type;

  float accuracy_drop;
  bool priority;
  attack_func_ptr side_effect;
  int condition;
  int chance;
} attack;

extern attack empty_attack;

extern attack tackle;
extern attack scratch;
extern attack growl;
extern attack tail_whip;
extern attack string_shot;

extern attack poison_sting;
extern attack string_shot;
extern attack sand_attack;
extern attack quick_attack;
extern attack defense_curl;
extern attack vine_whip;

//// attacks.c ////

//Handle all operations for an attack, indicate whether this is an enemy attack (true)
void perform_attack(struct pokemon *perp, int move_num, struct pokemon *victim, bool enemy);

//Get damage that should be dealt with a given move from one pokemon to another
int getDamage(struct pokemon *perp, int move_num, struct pokemon *victim);




//// attacks_list.c ////

//Return an attack given an attack id number
attack * get_attack_by_id(int id_num);

//Certain attacks can increment a pokemon's own stat
int increment_self_stat(Condition stat_type, int chance, struct pokemon* victim);

//Certain attacks can decrement an enemy pokemon's stat
int decrement_opponent_stat(Condition stat_type, int chance, struct pokemon* victim);

#endif // ATTACKS_H