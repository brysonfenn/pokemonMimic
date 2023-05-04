#include "pokemon.h"

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../print_utils.h"
#include "../print_defines.h"

#define LINE_SIZE 100

void randomize_stats(pokemon * new_pok, int level, int level_min, int level_max);
void learn_move(pokemon * pok, attack * new_attack);

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
  new_pok->baseSpeed += (level + 2) + (rand() % 3);
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
  printw("%s  LVL %d:\n", pok->name, pok->level);
  printw("EXP to next Level: %d\n\n", (pok->level * 8) - pok->exp);
  printw("HP: %d/%d", pok->currentHP, pok->maxHP);
  if (!(pok->currentHP)) printw("  (Fainted)");
  printw("\nBase Attack: %d\nBase Defense: %d\n", pok->baseAttack, pok->baseDefense);
  printw("Base Speed: %d\n\n", pok->baseSpeed);
  printw("Attacks: \n");
  mvprintw(9, 8, "%s", pok->attacks[0].name);
  mvprintw(9, 25,"%s", pok->attacks[1].name);
  mvprintw(10,8, "%s", pok->attacks[2].name);
  mvprintw(10,25,"%s\n", pok->attacks[3].name);
  // printw("\t%s\r\t\t\t%s\n\t%s\r\t\t\t%s\n", pok->attacks[0].name, pok->attacks[1].name, pok->attacks[2].name, pok->attacks[3].name);
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

  text_box_cursors(TEXT_BOX_BEGINNING);
  printw("%s has grown to level %d!\n", pok->name, pok->level);
  refresh(); sleep(2);

  //Add moves
  FILE *fp;
  char filename[50];
  sprintf(filename, "learnsets/%03d_%s.txt", pok->id_num, pok->name);
  char line[LINE_SIZE];

  // Open the file for reading
  fp = fopen(filename, "r");

  // Check if the file was opened successfully
  if (fp == NULL) {
      printw("Learnset file does not exist.\n"); refresh(); sleep(2);
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
      learn_move(pok, &new_attack);
    }
  }

  fclose(fp);
}

void learn_move(pokemon * pok, attack * new_attack) {
  int input_num;

  //If there are not 4 moves, auto-learn, if not, forget.
  if (pok->numAttacks < 4) {
    pok->attacks[pok->numAttacks] = *new_attack;
    pok->numAttacks++;
  }
  else {
    mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  %s", pok->attacks[0].name); 
    mvprintw(SELECT_Y,BATTLE_SELECT_2_X,"  %s", pok->attacks[1].name); 
    mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  %s", pok->attacks[2].name); 
    mvprintw(SELECT_Y+1,BATTLE_SELECT_2_X,"  %s", pok->attacks[3].name); 
    mvprintw(SELECT_Y+1,BATTLE_SELECT_3_X,"  Cancel");

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("\n%s wants to learn %s, but %s already knows 4 moves.", pok->name, new_attack->name, pok->name); 
    refresh(); sleep(2);

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("Select a move to forget.");
        
    input_num = get_fight_selection(SELECT_Y, pok->numAttacks);

    text_box_cursors(TEXT_BOX_NEXT_LINE);

    if (input_num == 5) {
      printw("%s did not learn %s!\n", pok->name, new_attack->name); refresh(); sleep(2); return;
    }
    printw("1...2...and...poof!\n"); refresh(); sleep(2);
    printw("%s forgot %s, and...\n", pok->name, pok->attacks[input_num]); refresh(); sleep(2);
    pok->attacks[input_num] = *new_attack;
  }

  printw("%s learned %s!\n", pok->name, new_attack->name); refresh(); sleep(2);
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
      printw("Learnset file does not exist.\n"); refresh(); sleep(2);
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