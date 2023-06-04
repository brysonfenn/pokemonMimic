#include "pokemon.h"

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../player.h"

//Give random stats to pokemon based on level
  //level = particular level
  //if level = 0, get random level from range level_min:level_max
void calculate_stats(Pokemon * pok, int level, int level_min, int level_max) {
  //If level is zero, get a level within the given range
  if (!level) {
    level = (rand() % (level_max + 1 - level_min)) + level_min;
  }
  pok->level = level;
  int ev = 1;

  int hp_iv = pokemon_get_iv(pok, IV_HP);
  int atk_iv =pokemon_get_iv(pok, IV_ATTACK);
  int def_iv = pokemon_get_iv(pok, IV_DEFENSE);
  int sp_atk_iv = pokemon_get_iv(pok, IV_SP_ATTACK);
  int sp_def_iv = pokemon_get_iv(pok, IV_SP_DEFENSE);
  int spd_iv = pokemon_get_iv(pok, IV_SPEED);

  pok->maxHP = (((2.0 * pok->maxHP + hp_iv + (ev/4)) * level) / 100) + level + 10;
  pok->baseAttack = (((2.0 * pok->baseAttack + atk_iv + (ev/4)) * level) / 100) + 5;
  pok->baseDefense = (((2.0 * pok->baseDefense + def_iv + (ev/4)) * level) / 100) + 5;
  pok->baseSpAttack = (((2.0 * pok->baseSpAttack + sp_atk_iv + (ev/4)) * level) / 100) + 5;
  pok->baseSpDefense = (((2.0 * pok->baseSpDefense + sp_def_iv + (ev/4)) * level) / 100) + 5;
  pok->baseSpeed = (((2.0 * pok->baseSpeed + spd_iv + (ev/4)) * level) / 100) + 5;

  reset_stat_stages(pok);
}

//Reset stat stages whenever pokemon is switched out or battle ends
void reset_stat_stages(Pokemon *pok) {
  pok->atk_stage = 0;
  pok->def_stage = 0;
  pok->sp_atk_stage = 0;
  pok->sp_def_stage = 0;
  pok->spd_stage = 0;
  pok->acc_stage  = 0;
  pok->evade_stage = 0;
  pok->confusion_count = 0;
  remove_all_hidden_conditions(pok);
}


void print_pokemon_attacks(Pokemon *pok, int first_line) {
  mvprintw(first_line, POKE_SUMMARY_ATKS_X, "%s", pok->attacks[0].name);
  mvprintw(first_line, POKE_SUMMARY_ATKS_X+MOVE_SELECT_SPACING, "%s", pok->attacks[1].name);
  mvprintw(first_line+1, POKE_SUMMARY_ATKS_X, "%s", pok->attacks[2].name);
  mvprintw(first_line+1, POKE_SUMMARY_ATKS_X+MOVE_SELECT_SPACING, "%s", pok->attacks[3].name);
  refresh();
}

//Print stats, attacks, etc of a given pokemon
void print_pokemon_summary(Pokemon *pok) {
  char print_str[8192];

  sprintf(print_str, "%s  LVL %d\n\t%s ", pok->name, pok->level, get_type_string_by_id(pok->type1));
  if (pok->type2 != NO_TYPE) sprintf(print_str, "%s%s", print_str, get_type_string_by_id(pok->type2));
  sprintf(print_str, "%s\n", print_str);
  sprintf(print_str, "%sEXP to next Level: %d\n", print_str, (pok->level * 8) - pok->exp);
  sprintf(print_str, "%sHP: %d/%d", print_str, pok->currentHP, pok->maxHP);
  if (!(pok->currentHP)) sprintf(print_str, "%s  (Fainted)", print_str);
  sprintf(print_str, "%s\n \nATTACK: %d\nDEFENSE: %d\n", print_str, pok->baseAttack, pok->baseDefense);
  sprintf(print_str, "%sSP ATTACK: %d\nSP DEFENSE: %d\n", print_str, pok->baseSpAttack, pok->baseSpDefense);
  sprintf(print_str, "%sSPEED: %d\n", print_str, pok->baseSpeed);
  sprintf(print_str, "%s \nAttacks: \n", print_str);

  print_to_list(print_str);

  print_pokemon_attacks(pok, POKE_SUMMARY_ATKS_BEGIN);
}

//Control all actions for the pokemon menu
int handle_pokemon_menu(int input_num1) {
  int input_num2;
  char print_str[256];
  Pokemon tempPok;

  begin_list();
  print_to_list("  Switch\n  Release\n  Attacks\n  Cancel\n--------------------------------------------------------");
  print_pokemon_summary(&(player.party[input_num1]));
  input_num2 = get_selection(LIST_BOX_Y+1, 3, 0);

  //Break if inputNum2 is 2 (cancel)
  if (input_num2 == 3 || input_num2 == PRESSED_B) { return RETURN_TO_PARTY; }
  //Switch
  else if (input_num2 == 0) {
      begin_list();
      if (player.numInParty <= 1) { print_to_list("You only have 1 Pokémon!\n"); sleep(2); return RETURN_TO_SUMMARY; }
      sprintf(print_str, "Which pokemon would you like to switch with %s?\n", player.party[input_num1].name);
      print_to_list(print_str);
      printParty();
      print_to_list("  Cancel");
      input_num2 = get_selection(LIST_BOX_Y+3, player.numInParty, input_num1);

      if (input_num2 == player.numInParty || input_num2 == PRESSED_B) { return RETURN_TO_SUMMARY; }
      if (input_num2 == input_num1) { return RETURN_TO_SUMMARY; }

      tempPok = player.party[input_num1];
      player.party[input_num1] = player.party[input_num2];
      player.party[input_num2] = tempPok;

      sprintf(print_str, " \n \nB switched %s with %s!", player.party[input_num2].name, player.party[input_num1].name);
      print_to_list(print_str); sleep(2);
  }
  //Release
  else if (input_num2 == 1) {
      begin_list();
      if (player.numInParty <= 1) { print_to_list("You only have 1 Pokémon!\n"); sleep(2); return RETURN_TO_SUMMARY; }
      sprintf(print_str, "Are you sure you want to release %s?\n  Yes\n  No\n", player.party[input_num1].name);
      print_to_list(print_str);
      input_num2 = get_selection(LIST_BOX_Y+2, 1, 0);
      if (input_num2 == 1 || input_num2 == PRESSED_B) { return RETURN_TO_SUMMARY; }
      else {
      sprintf(print_str, "Bye Bye, %s!\n", player.party[input_num1].name);
      print_to_list(print_str); sleep(2);
      player.numInParty--;
      for (int i = input_num1; i < player.numInParty; i++) {
          player.party[i] = player.party[i+1];
      }
      player.party[player.numInParty] = emptyPok;
      }
  }
  //View attack stats
  else if (input_num2 == 2) {
    input_num2 = 0;
    while (input_num2 != 5) {
      tempPok = player.party[input_num1];
      begin_list();
      sprintf(print_str, "%s Attacks:", tempPok.name);
      print_to_list(print_str);
      print_pokemon_attacks(&(player.party[input_num1]), LIST_BOX_Y+3);
      mvprintw(LIST_BOX_Y+4,POKE_SUMMARY_ATKS_X+MOVE_SELECT_SPACING*2, "  Cancel");
      input_num2 = 
        get_move_selection(POKE_SUMMARY_ATKS_X-2, LIST_BOX_Y+3, &(player.party[input_num1]));
    }
    return RETURN_TO_SUMMARY;
  }

  return RETURN_TO_MENU;
}

//Return the fraction modifier for a stat given the stage (stage 0 return 1.0)
float get_stat_modifier(int16_t stage) {
  float modified_stat = 1.0;
  if (stage >= 0) { modified_stat = (2.0 + stage) / 2.0; }  // positive means 3/2, 4/2, etc.
  if (stage < 0) { modified_stat = 2.0 / (2.0 - stage); }   // negative means 2/3, 2/4, etc.
  return modified_stat;
}

//Get the catch rate for a pokemon with pok_id
int pokemon_get_catch_rate(int pok_id) {

  char line[LINE_SIZE];

  //only allow pok_id's for pokemon between 1 and 151
  if (pok_id > 151 || pok_id < 1) {
    begin_list();
    sprintf(line, "catch rate not available for id # %d", pok_id);
    print_to_list(line);
    return 127;
  }

  FILE *fp;
  char filename[50];
  sprintf(filename, "catch_rates.txt", pok_id);

  // Open the file for reading
  fp = fopen(filename, "r");

  // Check if the file was opened successfully
  if (fp == NULL) {
      printw("Catch Rate file does not exist.\n"); refresh(); sleep(2);
      return;
  }

  for (int i = 0; i <= pok_id; i++) {
    fgets(line, LINE_SIZE, fp);	// keep getting lines up to pok_id
  }

  int curr_id, catch_rate;
  char name[256];
  
  sscanf(line, "%d %[^,], %d", &curr_id, &name, &catch_rate);

  return catch_rate;
}


int pokemon_get_accuracy(Pokemon * perp, int move_num, Pokemon * victim) {
  float modified_accuracy = (float) (perp->attacks[move_num].accuracy);

  //Accuracy is determined based on perp's accuracy and victim's evasiveness
  //The difference can only be +6 at most or -6 at least
  float stage = (float) perp->acc_stage - victim->evade_stage;
  if (stage > 6) stage = 6;
  if (stage < -6) stage = -6;

  if (stage >= 0) { modified_accuracy *= ((3.0 + stage) / 3.0); }  // positive means 4/3, 5/3, etc.
  if (stage < 0) { modified_accuracy *= (3.0 / (3.0 - stage)); }   // negative means 3/4, 3/5, etc.

  int accuracy = (int) modified_accuracy;
  if (accuracy < 5) accuracy = 5;
  if (accuracy > 100) accuracy = 100;

  return accuracy;
}