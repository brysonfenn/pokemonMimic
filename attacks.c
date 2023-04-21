#include "attacks.h"

#include "pokemon.h"

#define NUM_ATTACKS 50

attack empty_attack=  {"------------",  0,  0, 100, 0, 0, 0, 0 };
//				 	           "Name        "  id  pwr acc  ad dd sd acd  pmove
attack tackle       = {"Tackle      ",  1,  6,  95, 0, 0, 0, 0.0, false, &attack_do_nothing };
attack scratch      = {"Scratch     ",  2,  5, 100, 0, 0, 0, 0.0, false };
attack growl        = {"Growl       ",  3,  0, 100, 1, 0, 0, 0.0, false };
attack tail_whip    = {"Tail Whip   ",  4,  0, 100, 0, 1, 0, 0.0, false };
attack string_shot  = {"String Shot ",  5,  0,  95, 0, 0, 1, 0.0, false };
attack poison_sting = {"Poison Sting",  6,	3, 100, 0, 0, 0, 0.0, false };
attack sand_attack  = {"Sand Attack ",  7,	0, 100, 0, 0, 0, 0.1, false };
attack quick_attack = {"Quick Attack",  8,  6, 100, 0, 0, 0, 0.0,  true };

static attack * local_array[NUM_ATTACKS] = { &empty_attack, &tackle, &scratch, &growl, &tail_whip, &string_shot, &poison_sting,
                                              &sand_attack };

void attacks_init() {
	
}

attack * get_attack_by_id(id_num) {
	return local_array[id_num];
}

void perform_attack(struct pokemon *perp, int move_num, struct pokemon *victim, bool enemy) {
  int attack_power;

  attack chosenAttack = perp->attacks[move_num];

  if (enemy) printf("\n\n\n\nEnemy ");
  printf("%s used %s\n", perp->name, chosenAttack.name);
  sleep(1);

  //Calculate current accuracy
  int curr_accuracy =  perp->accuracy * chosenAttack.accuracy;
  bool missed = (rand() % 100) >= curr_accuracy;

  if (missed) {
    if (chosenAttack.power) {
      if (enemy) printf("Enemy ");
      printf("%s's attack missed.\n", perp->name);
    }
    else {
      printf("But it failed!\n");
    }
    sleep(2); return;
  }

  //Drop HP only if attack has damage power
  if (perp->attacks[move_num].power != 0) {
    attack_power = (perp->attacks[move_num].power + perp->currAttack - victim->currDefense);
    attack_power = (attack_power <= 0) ? 1 : attack_power;  // Pokemon should always be able to do 1 damage
    victim->currentHP -= attack_power;
  }

  //Drop Attack unless we are already 6 below baseAttack
  if (chosenAttack.attack_drop) {
    if (!enemy) printf("Enemy ");
    if ((victim->baseAttack - victim->currAttack) >= 6) {
      printf("%s's attack won't go any lower!\n", victim->name);
    }
    else {
      printf("%s's attack fell\n", victim->name); 
      victim->currAttack -= chosenAttack.attack_drop;
    }
    sleep(1);
  }

  //Drop Defense unless we are already 6 below baseDefense
  if (chosenAttack.defense_drop) {
    if (!enemy) printf("Enemy ");
    if ((victim->baseDefense - victim->currDefense) >= 6) {
      printf("%s's defense won't go any lower!\n", victim->name);
    }
    else {
      printf("%s's defense fell\n", victim->name); 
      victim->currDefense -= chosenAttack.defense_drop;
    }
    sleep(1);
  }

  //Drop Speed unless we are already 6 below baseSpeed
  if (chosenAttack.speed_drop) {
    if (!enemy) printf("Enemy ");
    if ((victim->baseSpeed - victim->currSpeed) >= 6) {
      printf("%s's speed won't go any lower!\n", victim->name);
    }
    else {
      printf("%s's speed fell\n", victim->name); 
      victim->currSpeed -= chosenAttack.speed_drop;
    }
    sleep(1);
  }

  //Drop Accuracy unless we are already at 0.4 accuracy
  if (chosenAttack.accuracy_drop) {
    if (!enemy) printf("Enemy ");
    if (victim->accuracy <= 0.4) {
      printf("%s's accuracy won't go any lower!\n", victim->name);
    }
    else {
      printf("%s's accuracy fell\n", victim->name); 
      victim->accuracy -= chosenAttack.accuracy_drop;
    }
    sleep(1);
  }

  sleep(1);
}
