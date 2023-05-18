#include "attacks.h"

#include <ncurses.h> 

#include "pokemon.h"
#include "../print_utils.h"
#include "../print_defines.h"

int perform_struggle(struct pokemon *perp, struct pokemon *victim, bool enemy);

//Handle all operations for an attack, indicate whether this is an enemy attack (true)
int perform_attack(struct pokemon *perp, int move_num, struct pokemon *victim, bool enemy) {

  //Handle sleep
  text_box_cursors(TEXT_BOX_BEGINNING);
  if (perp->visible_condition == ASLEEP) {
    if (enemy) printw(ENEMY_TEXT);
    if (perp->sleep_count > 0) {
      printw("%s is fast asleep...", perp->name);
      refresh(); sleep(2);
      perp->sleep_count--;
      return 0;
    }
    else {
      printw("%s woke up!", perp->name);
      perp->visible_condition = NO_CONDITION;
      refresh(); sleep(2);
      clear(); printBattle();
    }
  }

  //Handle Paralysis
  if (perp->visible_condition == PARALYZED) {
    if ((rand() % 100) < 25) {
      printw("%s is paralyzed, it can't move!", perp->name);
      refresh(); sleep(2);
      return 0;
    }
  }

  //Struggle
  if (move_num == -1) {
    perform_struggle(perp, victim, enemy);
    return 0;
  }

  //Decrement pp and assign attack
  perp->attacks[move_num].curr_pp--;
  attack chosenAttack = perp->attacks[move_num];

  //Calculate current accuracy
  int curr_accuracy =  pokemon_get_accuracy(perp, move_num, victim);
  bool missed = (rand() % 100) >= curr_accuracy;

  //Use attack
  text_box_cursors(TEXT_BOX_BEGINNING);
  if (enemy) printw(ENEMY_TEXT);
  printw("%s used %s! Accuracy: %d%%", perp->name, chosenAttack.name, curr_accuracy);
  refresh(); sleep(1);

  //Check if missed
  if (missed) {
    text_box_cursors(TEXT_BOX_NEXT_LINE);
    if (chosenAttack.power) {
      if (enemy) printw(ENEMY_TEXT);
      printw("%s's attack missed.", perp->name);
    }
    else {
      printw("But it failed!");
    }
    refresh(); sleep(2); return 0;
  }

  //Drop HP only if attack has damage power
  if (chosenAttack.power != 0) {
    int damage = getDamage(perp, move_num, victim, true);
    victim->currentHP -= damage;
  }

  sleep(1);

  chosenAttack.side_effect(chosenAttack.condition, chosenAttack.chance, victim);

  return 0;
}


//Get damage that should be dealt with a given move from one pokemon to another
int getDamage(struct pokemon *perp, int move_num, struct pokemon *victim, bool print_statements) {
  attack chosenAttack = perp->attacks[move_num];

  int perpAttack, victimDefense, perpAtkStage, victimDefStage;
  Type moveType = perp->attacks[move_num].type;

  //See whether we will use physical or special
  if ( moveType == NORMAL || moveType == FIGHTING || moveType == POISON || moveType == GROUND || 
        moveType == FLYING || moveType == BUG || moveType == ROCK || moveType == GHOST ) 
  {
    //Physical Stats
    perpAttack = perp->baseAttack; victimDefense = victim->baseDefense;
    perpAtkStage = perp->atk_stage; victimDefStage = victim->def_stage;
  } 
  else 
  {
    //Special Stats
    perpAttack = perp->baseSpAttack; victimDefense = victim->baseSpDefense;
    perpAtkStage = perp->sp_atk_stage; victimDefStage = victim->sp_def_stage;
  }

  // Basic Equation
  float damage_f = ( ((2.0 * perp->level / 5) + 2) * chosenAttack.power * perpAttack / victimDefense) / 50.0;
  float random_float = (float) ((rand() % 25) + 85); // Random number between 85 and 110
  damage_f *= (random_float / 100.0); // Randomize damage a bit
  damage_f += 2;

  // Damage modifier stages
  float perp_attack_modifier = get_stat_modifier(perpAtkStage);
  float victim_defense_modifier = get_stat_modifier(victimDefStage);
  damage_f = damage_f * (perp_attack_modifier / victim_defense_modifier);

  int damage = (int) damage_f;

  damage = (damage <= 0) ? 1 : damage;  // Pokemon should always be able to do 1 damage

  //Critical Hit 1/16 Chance - Only apply if we are printing (getting damage for an attack)
  //This should probably check for doesn't affect
  if ((rand() % 16) == 0 && print_statements) {
    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("A critical hit!"); refresh(); sleep(2);
    damage *= 2;
  }

  //Calculate effectiveness
  damage = get_damage_after_effectiveness(chosenAttack.type, victim, damage, print_statements);

  return damage;
}

int perform_struggle(struct pokemon *perp, struct pokemon *victim, bool enemy) {
  clear();
  printBattle();

  text_box_cursors(TEXT_BOX_BEGINNING);
  if (enemy) printw(ENEMY_TEXT);
  printw("%s used Struggle!", perp->name);
  refresh(); sleep(2);
  perp->currentHP -= (perp->maxHP / 16 + 1);
  victim->currentHP -= (perp->maxHP / 16 + 1);
}