#include "pokemon.h"

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../print_utils.h"
#include "../print_defines.h"

//Initialize a given pokemon new_pok and get randomized stats
  //level = particular level
  //if level = 0, get random level from range level_min:level_max
void pokemon_init(pokemon * new_pok, int level, int level_min, int level_max) {
  new_pok->exp = 15;
  new_pok->numAttacks = 0;
  new_pok->visible_condition = NO_CONDITION;
  new_pok->hidden_condition = NO_CONDITION;

  calculate_stats(new_pok, level, level_min, level_max);

  new_pok->currentHP = new_pok->maxHP;
  
  pokemon_give_moves(new_pok);
}

//Give random stats to pokemon based on level
  //level = particular level
  //if level = 0, get random level from range level_min:level_max
void calculate_stats(pokemon * new_pok, int level, int level_min, int level_max) {
  if (!level) {
    level = (rand() % (level_max - level_min)) + level_min;
  }
  new_pok->level = level;
  int iv = 31;
  int ev = 1;
  int hp = new_pok->maxHP;
  int att = new_pok->baseAttack;
  int def = new_pok->baseDefense;
  int spd = new_pok->baseSpeed;
  new_pok->maxHP = (((2 * new_pok->maxHP + iv + (ev/4)) * level) / 100) + level + 10;
  new_pok->baseAttack = (((2 * new_pok->baseAttack + iv + (ev/4)) * level) / 100) + 5;
  new_pok->baseDefense = (((2 * new_pok->baseDefense + iv + (ev/4)) * level) / 100) + 5;
  new_pok->baseSpeed = (((2 * new_pok->baseSpeed + iv + (ev/4)) * level) / 100) + 5;

  reset_base_stats(new_pok);
}

//Reset stat stages whenever pokemon is switched out or battle ends
void reset_base_stats(pokemon *pok) {
  pok->atk_stage = 0;
  pok->def_stage = 0;
  pok->spd_stage = 0;
  pok->accuracy  = 1.0;
  pok->hidden_condition = NO_CONDITION;
}

//Print stats, attacks, etc of a given pokemon
void print_pokemon_summary(pokemon *pok) {
  printw("%s  LVL %d\n\t%s ", pok->name, pok->level, get_typing_by_id(pok->type1));
  if (pok->type2 != NO_TYPE) printw("%s", get_typing_by_id(pok->type2));
  printw("\n\n");
  printw("EXP to next Level: %d\n", (pok->level * 8) - pok->exp);
  printw("HP: %d/%d", pok->currentHP, pok->maxHP);
  if (!(pok->currentHP)) printw("  (Fainted)");
  printw("\nAttack Stat: %d\nDefense Stat: %d\n", pok->baseAttack, pok->baseDefense);
  printw("Speed Stat: %d\n\n", pok->baseSpeed);
  printw("Attacks: \n");
  mvprintw(10, 8, "%s", pok->attacks[0].name);
  mvprintw(10, 25,"%s", pok->attacks[1].name);
  mvprintw(11,8, "%s", pok->attacks[2].name);
  mvprintw(11,25,"%s\n", pok->attacks[3].name);
}

//Return the fraction modifier for a stat given the stage (stage 0 return 1.0)
float get_stat_modifier(int16_t stage) {
  float modified_stat = 1.0;
  if (stage >= 0) { modified_stat = (2.0 + stage) / 2.0; }  // positive means 3/2, 4/2, etc.
  if (stage < 0) { modified_stat = 2.0 / (2.0 - stage); }   // negative means 2/3, 2/4, etc.
  return modified_stat;
}

