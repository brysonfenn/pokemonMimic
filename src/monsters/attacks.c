#include "attacks.h"

#include "pokemon.h"

int getDamage(struct pokemon *perp, int move_num, struct pokemon *victim);

void attacks_init() {
	
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
  if (chosenAttack.power != 0) {
    damage = getDamage(perp, move_num, victim);
    victim->currentHP -= damage;
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
    printf("A critical hit!\n"); sleep(2);
    damage *= 2;
  }

  //Calculate effectiveness
  damage = handle_effectiveness(chosenAttack.type, victim, damage);

  return damage;
}
