#include "attacks.h"

#include <ncurses.h> 

#include "pokemon.h"
#include "../print_utils.h"
#include "../print_defines.h"

#define CRITICAL_HIT_FLAG 0x01

int perform_struggle(struct pokemon *perp, struct pokemon *victim, bool enemy);
int get_basic_damage(int perp_level, int attack_power, int perp_atk, int victim_def, int perp_atk_stage, int victim_def_stage);

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

  //Handle Confusion
  if (perp->hidden_condition == CONFUSED) {
    text_box_cursors(TEXT_BOX_BEGINNING);
    if (perp->confusion_count <= 0) {
      printw("%s snapped out of confusion!", perp->name); refresh(); sleep(2);
      perp->hidden_condition = NO_CONDITION;
    }
    else {
      perp->confusion_count--;
      printw("%s is confused...", perp->name); refresh(); sleep(2);
      //Hurt self in 50% of cases
      if (rand() % 2 == 0) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("%s hurt itself in its confusion!", perp->name); refresh(); sleep(2);

        //Player attacks itself with base power 40
        int damage = get_basic_damage(perp->level, 40, perp->baseAttack, perp->baseDefense, perp->atk_stage, perp->def_stage);
        perp->currentHP -= damage;
        return 0;
      }
    }
  }

  //Struggle if PP is zero
  if (move_num == -1) {
    perform_struggle(perp, victim, enemy);
    return 0;
  }

  //Decrement pp and assign attack
  perp->attacks[move_num].curr_pp--;
  attack chosenAttack = perp->attacks[move_num];


  //Calculate current accuracy
  int curr_accuracy;
  if (chosenAttack.accuracy != NO_MISS) {
    curr_accuracy =  pokemon_get_accuracy(perp, move_num, victim);
  }
  else {
    curr_accuracy = 100;
  }
  
  bool missed = (rand() % 100) >= curr_accuracy;

  //Use attack
  text_box_cursors(TEXT_BOX_BEGINNING);
  if (enemy) printw(ENEMY_TEXT);
  printw("%s used %s!", perp->name, chosenAttack.name);
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

  int damage = 0;
  int flags = 0; //These flags are used to detect critical hits + effectiveness

  //Drop HP only if attack has damage power
  if (chosenAttack.power != 0) {
    damage = get_damage(perp, move_num, victim, true, &flags);
    victim->currentHP -= damage;
    if (victim->currentHP < 0) victim->currentHP = 0;
    blinkPokemon(enemy);
  }

  //Print critical & effectiveness messages
  if (flags & CRITICAL_HIT_FLAG) {
    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("A critical hit!"); refresh(); sleep(2);
  }
  if (flags & SUPER_EFFECTIVE_FLAG) {
    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("It's super effective!"); refresh(); sleep(2);
  }
  if (flags & NOT_VERY_EFFECTIVE_FLAG) {
    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("It's not very effective..."); refresh(); sleep(2);
  }
  if (flags & DOES_NOT_AFFECT_FLAG) {
    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("It had no effect."); refresh(); sleep(2);
  }

  chosenAttack.side_effect(chosenAttack.condition, chosenAttack.chance, victim, damage);

  return 0;
}


//Get damage that should be dealt with a given move from one pokemon to another
int get_damage(struct pokemon *perp, int move_num, struct pokemon *victim, bool print_statements, int *flags) {
  attack chosenAttack = perp->attacks[move_num];

  int perpAttack, victimDefense, perpAtkStage, victimDefStage;
  Type moveType = perp->attacks[move_num].type;

  //See whether we will use physical or special attack and defense stats
  if ( moveType == NORMAL || moveType == FIGHTING || moveType == POISON || moveType == GROUND || 
        moveType == FLYING || moveType == BUG || moveType == ROCK || moveType == GHOST || moveType == STEEL)
  {
    //Physical Stats
    perpAttack = perp->baseAttack; victimDefense = victim->baseDefense;
    perpAtkStage = perp->atk_stage; victimDefStage = victim->def_stage;

    //Burned Status condition cuts attack in half
    if (perp->visible_condition == BURNED) perpAttack = perpAttack / 2;
  }
  else 
  {
    //Special Stats
    perpAttack = perp->baseSpAttack; victimDefense = victim->baseSpDefense;
    perpAtkStage = perp->sp_atk_stage; victimDefStage = victim->sp_def_stage;
  }


  // Baseline damage
  int damage = get_basic_damage(perp->level, chosenAttack.power, perpAttack, victimDefense, perpAtkStage, victimDefStage);

  damage = (damage <= 0) ? 1 : damage;  // Pokemon should always be able to do 1 damage

  //Critical Hit 1/16 Chance - Only apply if we are printing (getting damage for an attack)
  //This should probably check for doesn't affect
  if ((rand() % 16) == 0 && print_statements) {
    *flags |= CRITICAL_HIT_FLAG;
    damage *= 2;
  }

  //Calculate effectiveness
  damage = get_damage_after_effectiveness(chosenAttack.type, victim, damage, print_statements, flags);

  return damage;
}

int get_basic_damage(int perp_level, int attack_power, int perp_atk, int victim_def, int perp_atk_stage, int victim_def_stage) {
  //Basic Equation
  float damage_f = ( ((2.0 * perp_level / 5) + 2) * attack_power * perp_atk / victim_def) / 50.0;
  float random_float = (float) ((rand() % 25) + 85); // Random number between 85 and 110
  damage_f *= (random_float / 100.0); // Randomize damage a bit
  damage_f += 2;

  // Damage modifier stages
  float perp_attack_modifier = get_stat_modifier(perp_atk_stage);
  float victim_defense_modifier = get_stat_modifier(victim_def_stage);
  damage_f = damage_f * (perp_attack_modifier / victim_defense_modifier);

  int damage = (int) damage_f;

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