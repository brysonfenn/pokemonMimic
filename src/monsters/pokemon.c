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
  new_pok->iv = rand();
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
void calculate_stats(pokemon * pok, int level, int level_min, int level_max) {
  if (!level) {
    level = (rand() % (level_max - level_min)) + level_min;
  }
  pok->level = level;
  int ev = 1;

  int hp_iv = pokemon_get_iv(pok, IV_HP);
  int atk_iv =pokemon_get_iv(pok, IV_ATTACK);
  int def_iv = pokemon_get_iv(pok, IV_DEFENSE);
  int sp_atk_iv = pokemon_get_iv(pok, IV_SP_ATTACK);
  int sp_def_iv = pokemon_get_iv(pok, IV_SP_DEFENSE);
  int spd_iv = pokemon_get_iv(pok, IV_SPEED);

  pok->maxHP = (((2 * pok->maxHP + hp_iv + (ev/4)) * level) / 100) + level + 10;
  pok->baseAttack = (((2 * pok->baseAttack + atk_iv + (ev/4)) * level) / 100) + 5;
  pok->baseDefense = (((2 * pok->baseDefense + def_iv + (ev/4)) * level) / 100) + 5;
  pok->baseSpAttack = (((2 * pok->baseSpAttack + sp_atk_iv + (ev/4)) * level) / 100) + 5;
  pok->baseSpDefense = (((2 * pok->baseSpDefense + sp_def_iv + (ev/4)) * level) / 100) + 5;
  pok->baseSpeed = (((2 * pok->baseSpeed + spd_iv + (ev/4)) * level) / 100) + 5;

  reset_base_stats(pok);
}

//Reset stat stages whenever pokemon is switched out or battle ends
void reset_base_stats(pokemon *pok) {
  pok->atk_stage = 0;
  pok->def_stage = 0;
  pok->sp_atk_stage = 0;
  pok->sp_def_stage = 0;
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
  printw("\n\nATTACK: %d\nDEFENSE: %d\n", pok->baseAttack, pok->baseDefense);
  printw("SP ATTACK: %d\nSP DEFENSE: %d\n", pok->baseSpAttack, pok->baseSpDefense);
  printw("SPEED: %d\n\n", pok->baseSpeed);
  printw("Attacks: \n");
  mvprintw(POKE_SUMMARY_ATKS_BEGIN, 8, "%s", pok->attacks[0].name);
  mvprintw(POKE_SUMMARY_ATKS_BEGIN, 25,"%s", pok->attacks[1].name);
  mvprintw(POKE_SUMMARY_ATKS_BEGIN+1, 8, "%s", pok->attacks[2].name);
  mvprintw(POKE_SUMMARY_ATKS_BEGIN+1, 25,"%s\n", pok->attacks[3].name);
}

//Return the fraction modifier for a stat given the stage (stage 0 return 1.0)
float get_stat_modifier(int16_t stage) {
  float modified_stat = 1.0;
  if (stage >= 0) { modified_stat = (2.0 + stage) / 2.0; }  // positive means 3/2, 4/2, etc.
  if (stage < 0) { modified_stat = 2.0 / (2.0 - stage); }   // negative means 2/3, 2/4, etc.
  return modified_stat;
}

