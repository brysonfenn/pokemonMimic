#ifndef ATTACKS_H
#define ATTACKS_H

#include "stdint.h"
#include "stdbool.h"

struct pokemon;

typedef struct attack {
  char name[30];
  int id_num;
  int power; 
  uint8_t accuracy;
  int8_t attack_drop;
  int8_t defense_drop;
  int8_t speed_drop;
  float accuracy_drop;
  bool priority;
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


void attacks_init();

attack * get_attack_by_id(int id_num);

void perform_attack(struct pokemon *perp, int move_num, struct pokemon *victim, bool enemy);



#endif // ATTACKS_H