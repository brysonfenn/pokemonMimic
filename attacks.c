#include "attacks.h"

#include "pokemon.h"
#include "conditions.h"

#define NUM_ATTACKS 50

attack empty_attack=  {"------------",  0,  0, 100, 0, 0, 0, 0 };
//				 	           "Name        "  id  pp  pwr  acc  ad dd sd acd  pmove
attack tackle       = {"Tackle      ",  1, 35,  40,  95, 0, 0, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack scratch      = {"Scratch     ",  2, 35,  40, 100, 0, 0, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack growl        = {"Growl       ",  3, 40,   0, 100, 1, 0, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack tail_whip    = {"Tail Whip   ",  4, 30,   0, 100, 0, 1, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack string_shot  = {"String Shot ",  5, 40,   0,  95, 0, 0, 1, 0.0, false, &attack_do_nothing, 0, 0 };
attack poison_sting = {"Poison Sting",  6, 35,  15, 100, 0, 0, 0, 0.0, false, &inflict_condition, POISONED, 30 };
attack sand_attack  = {"Sand Attack ",  7, 15,   0, 100, 0, 0, 0, 0.1, false, &attack_do_nothing, 0, 0 };
attack quick_attack = {"Quick Attack",  8, 30,  40, 100, 0, 0, 0, 0.0,  true, &attack_do_nothing, 0, 0 };
attack defense_curl = {"Defense Curl",  9, 40,   0, 100, 0,-1, 0, 0.0, false, &attack_do_nothing, 0, 0 };

static attack * local_array[NUM_ATTACKS] = { &empty_attack, &tackle, &scratch, &growl, &tail_whip, 
                                              &string_shot, &poison_sting, &sand_attack, &quick_attack, &defense_curl };

void attacks_init() {
	
}

attack * get_attack_by_id(id_num) {
	return local_array[id_num];
}

void perform_attack(struct pokemon *perp, int move_num, struct pokemon *victim, bool enemy) {
  int damage;

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
    // damage = (perp->attacks[move_num].power + perp->currAttack - victim->currDefense);
    float damage_f = ( ((2.0 * perp->level / 5) + 2) * perp->attacks[move_num].power * perp->currAttack / victim->currDefense) / 50.0;
    damage_f += 2;

    damage = (int) damage_f;

    damage = (damage <= 0) ? 1 : damage;  // Pokemon should always be able to do 1 damage
    victim->currentHP -= damage;
  }

  //Drop Attack unless we are already 6 below baseAttack
  if (chosenAttack.attack_drop > 0) {
    if (!enemy) printf("Enemy ");
    if ((victim->baseAttack - victim->currAttack) >= 6) {
      printf("%s's attack won't go any lower!\n", victim->name);
    }
    else {
      printf("%s's attack fell\n", victim->name);
      victim->currAttack *= 0.5;
    }
    sleep(1);
  }

  //Drop Defense unless we are already 6 below baseDefense
  if (chosenAttack.defense_drop > 0) {
    if (!enemy) printf("Enemy ");
    if ((victim->baseDefense - victim->currDefense) >= 6) {
      printf("%s's defense won't go any lower!\n", victim->name);
    }
    else {
      printf("%s's defense fell\n", victim->name); 
      victim->currDefense *= 0.5;
    }
    sleep(1);
  }
  else if (chosenAttack.defense_drop < 0) {
    if (enemy) printf("Enemy ");
    if ((perp->baseDefense - perp->currDefense) <= -6) {
      printf("%s's defense won't go any higher!\n", perp->name);
    }
    else {
      printf("%s's defense rose\n", perp->name); 
      perp->currDefense *= 1.5;
    }
    sleep(1);
  }

  //Drop Speed unless we are already 6 below baseSpeed
  if (chosenAttack.speed_drop > 0) {
    if (!enemy) printf("Enemy ");
    if ((victim->baseSpeed - victim->currSpeed) >= 6) {
      printf("%s's speed won't go any lower!\n", victim->name);
    }
    else {
      printf("%s's speed fell\n", victim->name); 
      victim->currSpeed *= 0.5;
    }
    sleep(1);
  }

  //Drop Accuracy unless we are already at 0.4 accuracy
  if (chosenAttack.accuracy_drop > 0) {
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

  chosenAttack.side_effect(chosenAttack.condition, chosenAttack.chance, victim);
}
