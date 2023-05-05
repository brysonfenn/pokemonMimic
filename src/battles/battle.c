#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "ncurses.h"

#include "battle.h"
#include "../player.h"
#include "../monsters/conditions.h"
#include "../monsters/pokemon.h"

#include "../print_utils.h"
#include "../print_defines.h"

static enum display { MAIN, FIGHT, BAG, POKEMON } current_display = MAIN;
static enum decision {NONE, ATTACK, ITEM, SWITCH, RUN } current_decision = NONE;

static bool pokemon_needing_exp[6] = {false, false, false, false, false, false};

static bool run_success;
static char tempString[1024];

//Handle an enemy attacking the player's current pokemon
void perform_enemy_attack(pokemon * currentPok, pokemon * enemy, int attack_num);

//Get a move number randomly (moves with higher damage have a higher chance)
int get_move(pokemon * pok);

//Give all pokemon exp that need it, and level up
void handle_exp(int exp);

//Begin a Battle with a given pokemon
int initiate_battle(struct pokemon * enemyPoke) {
  int inputNum, max_input;
  bool enemy_attacks, fainted_switch;
  int return_execute, attack_num, enemy_attack_num, item_num, pokemon_selected, speed_difference;
  attack_num = item_num = pokemon_selected = speed_difference = 0;
  enemy_attacks = false;
  fainted_switch = false;
  run_success = false;

  pokemon enemy = *enemyPoke;
  pokemon *currentPok = player.current_pokemon;
  player.enemy_pokemon = &enemy;

  //First used pokemon needs exp
  for (int i = 0; i < player.numInParty; i++) {
    if (currentPok == &(player.party[i])) {
      pokemon_needing_exp[i] = true;
    }
  }

  while (1) {
    clear();

    // Allow loop to break if enemyHP is 0 or less
    if (enemy.currentHP <= 0) {
      enemy.currentHP = 0; clear();
      printBattle(); sleep(2);
      text_box_cursors(TEXT_BOX_BEGINNING);
      printw("%s%s fainted.", ENEMY_TEXT, enemy.name); refresh(); sleep(2);
      break;
    }

    // If current pokemon faints, select a different pokemon.
    if (currentPok->currentHP == 0) {
      sleep(2);
      printw("%s fainted. ", currentPok->name); refresh();
      player.numAlive--;
      
      // Handle White out
      if (player.numAlive == 0) {
        sleep(2);
        printw("\nB is out of usable pokemon... "); refresh(); sleep(3);
        printw("B whited out."); refresh(); sleep(5);
        return BATTLE_WHITE_OUT;
      }
      printw("\nYou must select a different pokemon."); refresh(); sleep(2);
      clear();
      fainted_switch = true;      //Do not allow an attack for this switch
      current_display = POKEMON;  //Force a Switch
    }

    inputNum = 999;

    current_decision = NONE;

    int last_selection = 1;

    ///////// GET DECISION /////////
    while (current_decision == NONE) {
      clear();

      enemy_attack_num = get_move(&enemy);

      switch (current_display) {
        
      case MAIN:
        printBattle();
        mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  Fight"); mvprintw(SELECT_Y,BATTLE_SELECT_2_X,"  Bag");
        mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  Pokémon"); mvprintw(SELECT_Y+1,BATTLE_SELECT_2_X,"  Run");
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("What will B do?");

        inputNum = get_battle_selection(SELECT_Y, last_selection);
        last_selection = inputNum;
        run_success = false;
        if (inputNum == 4) {
          current_decision = RUN;
          break;
        }
        current_display = inputNum; //In this case, this is already the correct enum position
        enemy_attacks = false;
        break;
        
      case FIGHT:
        printBattle();
        mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  %s", currentPok->attacks[0].name); 
        mvprintw(SELECT_Y,BATTLE_SELECT_2_X,"  %s", currentPok->attacks[1].name); 
        mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  %s", currentPok->attacks[2].name); 
        mvprintw(SELECT_Y+1,BATTLE_SELECT_2_X,"  %s", currentPok->attacks[3].name); 
        mvprintw(SELECT_Y+1,BATTLE_SELECT_3_X,"  Cancel");
        
        inputNum = get_fight_selection(SELECT_Y, currentPok->numAttacks);

        //Handle Cancel
        if (inputNum == 5) {
          current_display = MAIN;
          break;
        }
        inputNum--;
        attack_num = inputNum;
        current_decision = ATTACK;
        current_display = MAIN;
        break;
        
      case BAG:
        printBag();
        inputNum = get_selection(1,0,player.numInBag,0, NOT_MAIN_SELECT);
        if (inputNum == player.numInBag) {
          current_display = MAIN;
          break;
        }
        item_num = inputNum;
        current_decision = ITEM;
        current_display = MAIN;
        break;
        
      case POKEMON:
        max_input = player.numInParty-1;
        printw("Select a pokemon to use.\n");
        printParty();
        if (player.current_pokemon->currentHP != 0) {
          max_input++;
          printw("  Cancel\n");
        }
        printw("\n");
        inputNum = get_selection(2,0,max_input, 0, NOT_MAIN_SELECT);
        if (inputNum == player.numInParty) {
          current_display = MAIN;
          continue;
        }
        if (player.party[inputNum].currentHP == 0) {
          mvprintw(player.numInParty+3, 0, "%s can't fight anymore! Select a different pokemon.\n", player.party[inputNum].name);
          refresh(); sleep(2);
          continue;
        }
        pokemon_selected = inputNum;
        current_decision = SWITCH;
        current_display = MAIN;
        break;
      default:
        current_display = MAIN;
        break;
      }
    }
    ///////// END GET DECISION /////////


    ///////// EXECUTE DECISION /////////
    bool enemy_priority, player_priority;
    int player_speed, enemy_speed;

    switch (current_decision) {
    case NONE:
      break;
    case ATTACK:

      player_speed = (int) (currentPok->baseSpeed * get_stat_modifier(currentPok->spd_stage));
      enemy_speed = (int) (enemy.baseSpeed * get_stat_modifier(enemy.spd_stage));
      
      //Handle Priority
      enemy_priority = enemy.attacks[enemy_attack_num].priority;
      player_priority = currentPok->attacks[attack_num].priority;
      if (enemy_priority && !player_priority)       { speed_difference = -1; }
      else if (!enemy_priority && player_priority)  { speed_difference =  1; }
      else { speed_difference = player_speed - enemy_speed; }
      
      //Select random first turn if pokemon speeds are equal
      if (speed_difference == 0) {
        speed_difference = (rand() % 2) ? 1 : -1;
      }

      //Pokemon with the higher speed goes first
      if (speed_difference > 0) {
        perform_attack(currentPok, attack_num, &(enemy), false);
        enemy_attacks = true;
      }
      else if (speed_difference < 0) {
        perform_enemy_attack(currentPok, &enemy, enemy_attack_num);
        clear();
        printBattle();
        //Player Pokemon can only attack if still alive
        if (currentPok->currentHP > 0) {
          perform_attack(currentPok, attack_num, &(enemy), false);
        }
      }
      break;
    case ITEM:
      return_execute = use_item(inputNum, &enemy);
      if (return_execute == ITEM_FAILURE) { continue; }
      else if (return_execute == ITEM_CATCH_SUCCESS) { run_success = true; }
      break;
    case SWITCH:
      if (currentPok != &(player.party[pokemon_selected])) {
        //If pokemon faints, it doesn't get exp
        if (fainted_switch) { 
          for (int i = 0; i < player.numInParty; i++) {
            if (currentPok == &(player.party[i])) {
              pokemon_needing_exp[i] = false;
            }
          }
        }

        //Make switch
        reset_base_stats(currentPok);
        set_current_pokemon(pokemon_selected);
        currentPok = player.current_pokemon;
        enemy_attacks = true;

        //Advise
        clear();
        printBattle();
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("B sent out %s!", player.current_pokemon->name); refresh(); sleep(2);

        // New pokemon needs exp
        pokemon_needing_exp[pokemon_selected] = true;
      }
      break;
    case RUN:
      if (player.trainer_battle) {
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("You can't run from a trainer battle!"); refresh(); sleep(2);
        enemy_attacks = false;
      }
      else {
        run_success = runAttempt();
        enemy_attacks = true;
      }
      break;
    default:
      break;
    }
    /////// END EXECUTE DECISION ///////

    if (run_success) break;  //Handle Run Away

    // Allow loop to break if enemyHP is 0 or less
    if (enemy.currentHP <= 0) {
      enemy.currentHP = 0; clear();
      printBattle(); sleep(2);
      text_box_cursors(TEXT_BOX_BEGINNING);
      printw("Enemy %s fainted.", enemy.name); refresh(); sleep(2);
      break;
    }

    // Enemy performs attack if we just did something
    if (enemy_attacks && !fainted_switch) {
      perform_enemy_attack(currentPok, &enemy, enemy_attack_num);
    }
    fainted_switch = false;

    handle_end_conditions();

    clear();
  }

  if (!run_success) {
    handle_exp(enemy.level * 3);
  }
  
  return BATTLE_WIN;
}


//Handle an enemy attacking the player's current pokemon
void perform_enemy_attack(pokemon * currentPok, pokemon * enemy, int attack_num) {
  clear();
  printBattle();

  perform_attack(enemy, attack_num, currentPok, true);
  if (currentPok->currentHP <= 0) {
    currentPok->currentHP = 0;
    clear();
    printBattle();
  }
}


//Get a move number randomly (moves with higher damage have a higher chance)
int get_move(pokemon * pok) {

  int max_move_index = 0;
  int move =  0;

  //Get move with the maximum power.
  for (int i = 0; i < pok->numAttacks; i++) {
    if (pok->attacks[i].power > pok->attacks[max_move_index].power) {
      max_move_index = i;
    }
  }

  int random = rand() % 100;
  if (random < 66) return max_move_index;
  else return (rand() % pok->numAttacks);
}


//Give all pokemon exp that need it, and level up
void handle_exp(int exp) {
  pokemon * currentPok;
  int next_level_exp;
  int num_exp_earned_pokemon = 0;

  for (int i = 0; i < 6; i++) { 
    if (pokemon_needing_exp[i]) num_exp_earned_pokemon++;
  }

  if (num_exp_earned_pokemon == 0) { printw("Problem in handle_exp()!\n"); refresh(); sleep(2); return; }

  exp = (exp / num_exp_earned_pokemon) + 1;

  for (int i = 0; i < 6; i++) {
    if (pokemon_needing_exp[i] == false) continue;

    currentPok = &(player.party[i]);

    //Give active pokemon experience points if it is alive and didn't run away.
    if (player.numAlive && !run_success) {
      text_box_cursors(TEXT_BOX_NEXT_LINE);
      printw("%s gained %d experience points!", currentPok->name, exp);
      currentPok->exp += (exp);
      refresh(); sleep(2);
    }

    next_level_exp = currentPok->level * 8;

    //Update levels
    while (currentPok->exp >= next_level_exp) {
      pokemon_level_up(currentPok, next_level_exp);
      next_level_exp = currentPok->level * 8;
    }
  }
  
  for (int i = 0; i < 6; i++) { pokemon_needing_exp[i] = false; }
}