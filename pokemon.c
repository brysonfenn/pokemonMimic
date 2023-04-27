#include "pokemon.h"
#include "stdio.h"
#include "unistd.h"
#include "stdbool.h"
#include "stdlib.h"

#include <string.h>

#define LINE_SIZE 100

void randomize_stats(pokemon * new_pok, int level, int level_min, int level_max);

void pokemon_init(pokemon * new_pok, int level, int level_min, int level_max) {
  randomize_stats(new_pok, level, level_min, level_max);
  new_pok->visible_condition = NO_CONDITION;
  new_pok->hidden_condition = NO_CONDITION;
  new_pok->numAttacks = 0;
  pokemon_give_moves(new_pok);
}

void randomize_stats(pokemon * new_pok, int level, int level_min, int level_max) {
  if (!level) {
    level = (rand() % (level_max - level_min)) + level_min;
  }
  new_pok->level = level;
  new_pok->exp = 0;
  new_pok->maxHP +=  (2*level) +  (rand() % 3);
  new_pok->currentHP = new_pok->maxHP;
  new_pok->baseAttack += (level + 2) + (rand() % 3);
  new_pok->baseDefense += (level + 3) + (rand() % 2);
  new_pok->baseSpeed += (level + 2) + (rand() % 5);
  reset_base_stats(new_pok);
}

void reset_base_stats(pokemon *pok) {
  pok->atk_stage = 0;
  pok->def_stage = 0;
  pok->spd_stage = 0;
  pok->accuracy  = 1.0;
  pok->hidden_condition = NO_CONDITION;
}

void print_pokemon_summary(pokemon *pok) {
  printf("%s  LVL %d:\n", pok->name, pok->level);
  printf("EXP to next Level: %d\n\n", (pok->level * 8) - pok->exp);
  printf("HP: %d/%d", pok->currentHP, pok->maxHP);
  if (!(pok->currentHP)) printf("  (Fainted)");
  printf("\nBase Attack: %d\nBase Defense: %d\n", pok->baseAttack, pok->baseDefense);
  printf("Base Speed: %d\n\n", pok->baseSpeed);
  printf("Attacks: \n");
  printf("\t%s\r\t\t\t%s\n\t%s\r\t\t\t%s\n", pok->attacks[0].name, pok->attacks[1].name, pok->attacks[2].name, pok->attacks[3].name);
}

float get_stat_modifier(int16_t stage) {
  float modified_stat = 1.0;
  if (stage >= 0) { modified_stat = (2.0 + stage) / 2.0; }  // positive means 3/2, 4/2, etc.
  if (stage < 0) { modified_stat = 2.0 / (2.0 - stage); }   // negative means 2/3, 2/4, etc.
  return modified_stat;
}

void pokemon_level_up(pokemon *pok, int next_level_exp) {
  pok->level++;
  pok->exp = (pok->exp - next_level_exp);

  //Upgrade stats
  pok->maxHP += 2; pok->currentHP += 2;
  pok->baseAttack++;
  pok->baseDefense++;
  pok->baseSpeed++;
  printf("%s has grown to level %d!\n", pok->name, pok->level);
  sleep(2);

  //Add moves
  FILE *fp;
  char filename[50];
  sprintf(filename, "learnsets/%03d_%s.txt", pok->id_num, pok->name);
  char line[LINE_SIZE];

  // Open the file for reading
  fp = fopen(filename, "r");

  // Check if the file was opened successfully
  if (fp == NULL) {
      printf("Learnset file does not exist.\n"); sleep(2);
      return;
  }

  // Read lines from the file and put them into game values
  fgets(line, LINE_SIZE, fp);	// Name first line
  fgets(line, LINE_SIZE, fp);	// Format second line

  int level_target, move_id;
  attack new_attack;

  while (fgets(line, LINE_SIZE, fp)) {
    // if (strstr(line, "end") != NULL) break;
    sscanf(line, "%d,%d", &level_target, &move_id);
    if (level_target == pok->level) {
      new_attack = *(get_attack_by_id(move_id));
      pok->attacks[pok->numAttacks] = new_attack;
      pok->numAttacks++;
      printf("%s learned %s\n", pok->name, new_attack.name); sleep(2);
    }
  }

  fclose(fp);
}

void pokemon_give_moves(pokemon *pok) {
  pok->numAttacks = 0;

  //Add moves
  FILE *fp;
  char filename[50];
  sprintf(filename, "learnsets/%03d_%s.txt", pok->id_num, pok->name);
  char line[LINE_SIZE];

  // Open the file for reading
  fp = fopen(filename, "r");

  // Check if the file was opened successfully
  if (fp == NULL) {
      printf("Learnset file does not exist.\n"); sleep(2);
      return;
  }

  // Read lines from the file and put them into game values
  fgets(line, LINE_SIZE, fp);	// Name first line
  fgets(line, LINE_SIZE, fp);	// Format second line

  int level_target, move_id;
  attack new_attack;
  uint8_t attack_position = 0;

  while (fgets(line, LINE_SIZE, fp)) {
    // if (strstr(line, "end") != NULL) break;
    sscanf(line, "%d,%d", &level_target, &move_id);
    if (level_target <= pok->level) {
      new_attack = *(get_attack_by_id(move_id));
      pok->attacks[attack_position] = new_attack;
      attack_position++;
      if (pok->numAttacks < 4) pok->numAttacks++;   //Only 4 attacks can exist
      if (attack_position > 3) attack_position = 0; //Cycle through position to get highest level attacks
    }
  }

  //Fill in with empty attacks if less than 4
  if (pok->numAttacks < 4) {
    for (int i = pok->numAttacks; i < 4; i++) {
      pok->attacks[i] = *(get_attack_by_id(0));
    }
  }

  fclose(fp);
}