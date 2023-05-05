#include "pokemon.h"

#include <stdio.h>

#include "../print_defines.h"
#include "../print_utils.h"

#define LINE_SIZE 100

void learn_move(pokemon * pok, attack * new_attack);
int evolve(pokemon * pok, int next_pok_id);

//Handle leveling up - also handles learning moves from new level
void pokemon_level_up(pokemon *pok, int next_level_exp) {
  pok->level++;
  pok->exp = (pok->exp - next_level_exp);

  //Upgrade stats
  pok->maxHP += 2; pok->currentHP += 2;
  pok->baseAttack++;
  pok->baseDefense++;
  pok->baseSpeed++;

  text_box_cursors(TEXT_BOX_BEGINNING);
  printw("%s has grown to level %d!", pok->name, pok->level);
  refresh(); sleep(2);

  //Add moves
  FILE *fp;
  char filename[50];
  sprintf(filename, "learnsets/id_%03d.txt", pok->id_num);
  char line[LINE_SIZE];
  char evolve_line[LINE_SIZE];

  // Open the file for reading
  fp = fopen(filename, "r");

  // Check if the file was opened successfully
  if (fp == NULL) {
      printw("Learnset file does not exist.\n"); refresh(); sleep(2);
      return;
  }

  // Read lines from the file and put them into game values
  fgets(line, LINE_SIZE, fp);	// Name first line
  fgets(evolve_line, LINE_SIZE, fp);  // Evolve second line
  fgets(line, LINE_SIZE, fp);	// Move Format third line

  int level_target, move_id, evolve_id;
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

  //Evolve if necessary
  sscanf(evolve_line, "Evolve: %d, %d", &level_target, &evolve_id);
  if (evolve_id == 0) { return; } //Handle no evolution
  else if (pok->level >= level_target) {
    evolve(pok, evolve_id);
  }
}

//Give a single move to a pokemon - also handle forgetting moves
void learn_move(pokemon * pok, attack * new_attack) {
  int input_num;

  //If there are not 4 moves, auto-learn, if not, forget.
  if (pok->numAttacks < 4) {
    pok->attacks[pok->numAttacks] = *new_attack;
    pok->numAttacks++;
  }
  else {

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("%s wants to learn %s,", pok->name, new_attack->name); 

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("but %s already knows 4 moves.", pok->name);
    refresh(); sleep(2);

    mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  %s", pok->attacks[0].name); 
    mvprintw(SELECT_Y,BATTLE_SELECT_2_X,"  %s", pok->attacks[1].name); 
    mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  %s", pok->attacks[2].name); 
    mvprintw(SELECT_Y+1,BATTLE_SELECT_2_X,"  %s", pok->attacks[3].name); 
    mvprintw(SELECT_Y+1,BATTLE_SELECT_3_X,"  Cancel");

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("Select a move to forget.");
        
    input_num = get_fight_selection(SELECT_Y, pok->numAttacks);
    input_num--;  //Adjust to array position

    text_box_cursors(TEXT_BOX_BEGINNING);

    if (input_num == 4) {
      printw("%s did not learn %s!", pok->name, new_attack->name); refresh(); sleep(2); return;
    }
    printw("1...2...and...poof!"); refresh(); sleep(2);

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("%s forgot %s, and...", pok->name, pok->attacks[input_num]); refresh(); sleep(2);
    pok->attacks[input_num] = *new_attack;
  }

  text_box_cursors(TEXT_BOX_NEXT_LINE);

  printw("%s learned %s!", pok->name, new_attack->name); refresh(); sleep(2);
}

//Give highest possible level moves to a pokemon
//This is called upon initializing a new pokemon
void pokemon_give_moves(pokemon *pok) {
  pok->numAttacks = 0;

  //Add moves
  FILE *fp;
  char filename[50];
  sprintf(filename, "learnsets/id_%03d.txt", pok->id_num);
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
  fgets(line, LINE_SIZE, fp);  // Evolve second line
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

//Return 0 is evolve success, return 1 if failed
int evolve(pokemon * pok, int next_pok_id) {
  char og_pok_name[LINE_SIZE];

  //Preserve some information like name and lost hp
  sprintf(og_pok_name, "%s", pok->name);
  int lost_hp = pok->maxHP - pok->currentHP;

  // Open the file for reading
  FILE *fp;
  char filename[50];
  sprintf(filename, "learnsets/id_%03d.txt", next_pok_id);
  char line[LINE_SIZE];
  fp = fopen(filename, "r");

  text_box_cursors(TEXT_BOX_BEGINNING);

  // Check if the file was opened successfully
  if (fp == NULL) {
      printw("Evolution Learnset file does not exist.\n"); refresh(); sleep(2);
      return 1;
  }

  printw("%s is evolving!", og_pok_name); refresh(); sleep(3);

  char type1[24];
	char type2[24];
  fgets(line, LINE_SIZE, fp);	// Name first line

  //Get new name, stats, and types
  sscanf(line, "%s %d %d %d %d %d %s %s", &(pok->name), &(pok->id_num), &(pok->maxHP), 
            &(pok->baseAttack), &(pok->baseDefense), &(pok->baseSpeed), type1, type2);
  pok->type1 = get_type_id_by_string(type1);
  pok->type2 = get_type_id_by_string(type2);

  randomize_stats(pok, pok->level, 0, 0);

  pok->currentHP = pok->maxHP - lost_hp;

  text_box_cursors(TEXT_BOX_NEXT_LINE);
  printw("Congratulations! %s evolved", og_pok_name);
  text_box_cursors(TEXT_BOX_NEXT_LINE);
  printw("into %s!", pok->name); refresh(); sleep(3);

  return 0;
}