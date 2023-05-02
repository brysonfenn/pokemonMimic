#include "attacks.h"

#include <ncurses.h> 

#include "pokemon.h"
#include "../print_utils.h"

int getDamage(struct pokemon *perp, int move_num, struct pokemon *victim);

void attacks_init() {
	
}

void perform_attack(struct pokemon *perp, int move_num, struct pokemon *victim, bool enemy) {
  int damage;

  attack chosenAttack = perp->attacks[move_num];

  move(10,0);

  if (enemy) printw("Enemy ");

  //Handle sleep
  if (perp->visible_condition == ASLEEP) {
    if (perp->sleep_count > 0) {
      printw("%s is fast asleep...\n", perp->name);
      refresh(); sleep(2);
      perp->sleep_count--;
      return;
    }
    else {
      printw("%s woke up!\n", perp->name);
      perp->visible_condition = NO_CONDITION;
      refresh(); sleep(2);
      clear(); printBattle();
    }
  }

  printw("%s used %s\n", perp->name, chosenAttack.name);
  refresh(); sleep(1);

  //Calculate current accuracy
  int curr_accuracy =  perp->accuracy * chosenAttack.accuracy;
  bool missed = (rand() % 100) >= curr_accuracy;

  if (missed) {
    if (chosenAttack.power) {
      if (enemy) printw("Enemy ");
      printw("%s's attack missed.\n", perp->name);
    }
    else {
      printw("But it failed!\n");
    }
    refresh(); sleep(2); return;
  }

  //Drop HP only if attack has damage power
  if (chosenAttack.power != 0) {
    damage = getDamage(perp, move_num, victim);
    victim->currentHP -= damage;
  }

  //Drop Accuracy unless we are already at 0.4 accuracy
  if (chosenAttack.accuracy_drop > 0) {
    if (!enemy) printw("Enemy ");
    if (victim->accuracy <= 0.4) {
      printw("%s's accuracy won't go any lower!\n", victim->name);
    }
    else {
      printw("%s's accuracy fell\n", victim->name); 
      victim->accuracy -= chosenAttack.accuracy_drop;
    }
    refresh(); sleep(1);
  }

  sleep(1);

  chosenAttack.side_effect(chosenAttack.condition, chosenAttack.chance, victim);
}


//Return how much damage should be caused
int getDamage(struct pokemon *perp, int move_num, struct pokemon *victim) {
  attack chosenAttack = perp->attacks[move_num];

  // Basic Equation
  float damage_f = ( ((2.0 * perp->level / 5) + 2) * chosenAttack.power * perp->baseAttack / victim->baseDefense) / 50.0;
  damage_f += 2;

  // Damage modifier stages
  float perp_attack_modifier = get_stat_modifier(perp->atk_stage);
  float victim_defense_modifier = get_stat_modifier(victim->def_stage);
  damage_f = damage_f * (perp_attack_modifier / victim_defense_modifier);

  int damage = (int) damage_f;

  damage = (damage <= 0) ? 1 : damage;  // Pokemon should always be able to do 1 damage

  //Critical Hit 1/16 Chance -- This should probably check for doesn't affect
  if ((rand() % 16) == 0) {
    printw("A critical hit!\n"); refresh(); sleep(2);
    damage *= 2;
  }

  //Calculate effectiveness
  damage = handle_effectiveness(chosenAttack.type, victim, damage);

  return damage;
}
