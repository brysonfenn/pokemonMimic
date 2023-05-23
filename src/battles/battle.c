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
#include "../items.h"
#include "../motion/motion2d.h"
#include "../motion/maps.h"

static enum display { MAIN, FIGHT, BAG, POKEMON } current_display = MAIN;
static enum decision {NONE, ATTACK, ITEM, SWITCH, RUN } current_decision = NONE;

static bool pokemon_needing_exp[6] = {false, false, false, false, false, false};

static bool run_success;

//Handle an enemyPok attacking the player's current pokemon
void perform_enemy_attack(pokemon * currentPok, pokemon * enemyPok, int attack_num);

//Get a move number randomly (moves with higher damage have a higher chance)
int get_move(pokemon * pok);

//Give all pokemon exp that need it, and level up
void handle_exp(int exp);

//Begin a Battle with a given pokemon
int initiate_battle(struct pokemon * enemyPok) {
  int inputNum, max_input;
  bool enemy_attacks, fainted_switch, out_of_pp;
  int return_execute, attack_num, enemy_attack_num, item_num, pokemon_selected, speed_difference;
  char print_str[512];

  attack_num = item_num = pokemon_selected = speed_difference = 0;
  enemy_attacks = false;
  fainted_switch = false;
  run_success = false;

  pokemon *currentPok = player.current_pokemon;
  player.enemy_pokemon = enemyPok;

  //First used pokemon needs exp
  for (int i = 0; i < player.numInParty; i++) {
    if (currentPok == &(player.party[i])) {
      pokemon_needing_exp[i] = true;
    }
  }

  while (1) {
    clear();

    // Allow loop to break if enemyHP is 0 or less
    if (enemyPok->currentHP <= 0) {
      enemyPok->currentHP = 0; clear();
      printBattle(); sleep(2);
      text_box_cursors(TEXT_BOX_BEGINNING);
      printw("%s%s fainted.", ENEMY_TEXT, enemyPok->name); refresh(); sleep(2);
      break;
    }

    // If current pokemon faints, select a different pokemon.
    if (currentPok->currentHP == 0) {
      printBattle();
      sleep(2);
      text_box_cursors(TEXT_BOX_BEGINNING);
      printw("%s fainted. ", currentPok->name); refresh(); sleep(2);
      
      // Handle White out
      if (player_get_num_alive() == 0) {
        sleep(2);
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("B is out of usable pokemon... "); refresh(); sleep(3);
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("B whited out."); refresh(); sleep(4);

        //Go to pokecenter and then heal
        back_to_poke_center();

        return BATTLE_WHITE_OUT;
      }

      text_box_cursors(TEXT_BOX_NEXT_LINE);
      printw("You must select a different pokemon."); refresh(); sleep(2);
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

      enemy_attack_num = get_move(enemyPok);

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
        //See if pokemon has pp
        out_of_pp = true;
        for (int i = 0; i < currentPok->numAttacks; i++) {
          if (currentPok->attacks[i].curr_pp > 0) {
            out_of_pp = false;
          }
        }
        if (out_of_pp) {
          current_display = MAIN;
          current_decision = ATTACK;
          attack_num = -1;
          break;
        }

        //If any pp, get attack choice
        printBattle();
        mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  %s", currentPok->attacks[0].name); 
        mvprintw(SELECT_Y,BATTLE_SELECT_1_X+MOVE_SELECT_SPACING,"  %s", currentPok->attacks[1].name); 
        mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  %s", currentPok->attacks[2].name); 
        mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X+MOVE_SELECT_SPACING,"  %s", currentPok->attacks[3].name); 
        mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X+MOVE_SELECT_SPACING*2,"  Cancel");
        
        inputNum = get_move_selection(BATTLE_SELECT_1_X, SELECT_Y, currentPok);

        //Handle Cancel
        if (inputNum == 5) {
          current_display = MAIN;
          break;
        }
        inputNum--;
        if (player.current_pokemon->attacks[inputNum].curr_pp == 0) {
          text_box_cursors(TEXT_BOX_BEGINNING);
          printw("There is no PP left for %s", currentPok->attacks[attack_num]); refresh(); sleep(2);
          current_display = MAIN;
          break;
        }

        attack_num = inputNum;
        current_decision = ATTACK;
        break;
        
      case BAG:
        begin_list();
        printBag();
        inputNum = get_selection(LIST_BOX_Y+2, 0, player.numInBag, 0);
        if (inputNum == player.numInBag || inputNum == PRESSED_B) {
          current_display = MAIN;
          break;
        }
        item_num = inputNum;

        current_decision = ITEM;
        break;
        
      case POKEMON:
        max_input = player.numInParty-1;
        begin_list();
        print_to_list("Select a pokemon to use.\n");
        printParty();
        if (player.current_pokemon->currentHP != 0) {
          max_input++;
          print_to_list("  Cancel\n");
        }
        inputNum = get_selection(LIST_BOX_Y+3, 0, max_input, 0);

        //Handle interesting 'pressed B' condition
        if (inputNum == PRESSED_B) {
          //If the cancel is an option (not fainted switch), treat as a cancel
          if (max_input == player.numInParty) inputNum = player.numInParty;
          //If this is a fainted switch, keep getting selection
          else continue;
        }

        if (inputNum == player.numInParty) {
          current_display = MAIN;
          continue;
        }
        if (player.party[inputNum].currentHP == 0) {
          sprintf(print_str, " \n%s can't fight anymore! \nSelect a different pokemon.\n", player.party[inputNum].name);
          print_to_list(print_str); sleep(2);
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
      //Get base attack
      player_speed = (int) (currentPok->baseSpeed * get_stat_modifier(currentPok->spd_stage));
      enemy_speed = (int) (enemyPok->baseSpeed * get_stat_modifier(enemyPok->spd_stage));

      //Handle paralysis
      if (currentPok->visible_condition == PARALYZED) {
        player_speed *= 0.25;
        player_speed = (player_speed <= 0) ? 1 : player_speed;
      }
      if (enemyPok->visible_condition == PARALYZED) {
        enemy_speed *= 0.25;
        enemy_speed = (enemy_speed <= 0) ? 1 : enemy_speed;
      }
      
      //Handle Priority
      enemy_priority = enemyPok->attacks[enemy_attack_num].priority;
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
        return_execute = perform_attack(currentPok, attack_num, enemyPok, false);
        if (return_execute == 0) enemy_attacks = true;
      }
      else if (speed_difference < 0) {
        perform_enemy_attack(currentPok, enemyPok, enemy_attack_num);
        clear();
        printBattle();
        clear_text_box();
        //Player Pokemon can only attack if still alive
        if (currentPok->currentHP > 0) {
          return_execute = perform_attack(currentPok, attack_num, enemyPok, false);
        }
      }

      current_display = MAIN;
      break;
    case ITEM:
      return_execute = use_item(inputNum, enemyPok);
      //Return to bag menu if item failed, else back to main menu
      if (return_execute == ITEM_FAILURE) { continue; }
      else if (return_execute == ITEM_CATCH_SUCCESS) { run_success = true; }
      else { enemy_attacks = true; }

      current_display = MAIN;
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
        reset_stat_stages(currentPok);
        set_current_pokemon(pokemon_selected);
        currentPok = player.current_pokemon;
        enemy_attacks = true;

        //Advise the player of the switch
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
    if (enemyPok->currentHP <= 0) {
      enemyPok->currentHP = 0; clear();
      printBattle(); sleep(2);
      text_box_cursors(TEXT_BOX_BEGINNING);
      printw("%s%s fainted.", ENEMY_TEXT, enemyPok->name); refresh(); sleep(2);
      break;
    }

    // Enemy performs attack if we just did something
    if (enemy_attacks && !fainted_switch) {
      perform_enemy_attack(currentPok, enemyPok, enemy_attack_num);
    }
    fainted_switch = false;

    handle_end_conditions();
  }

  if (!run_success) {
    int exp = enemyPok->level * 3;
    float random = (float) (rand() % 25);
    exp *= (1.0 + (random / 100.0));
    if (player.trainer_battle) exp *= 1.5;
    handle_exp(exp);
  }
  
  return BATTLE_WIN;
}


//Handle an enemyPok attacking the player's current pokemon
void perform_enemy_attack(pokemon * currentPok, pokemon * enemyPok, int attack_num) {
  clear();
  printBattle();
  perform_attack(enemyPok, attack_num, currentPok, true);

  if (currentPok->currentHP <= 0) {
    currentPok->currentHP = 0;
    clear();
    printBattle();
  }
}


//Get a move number randomly (moves with higher damage have a higher chance)
int get_move(pokemon * pok) {
  int num_available_attacks = 0;
  int available_attacks[4];
  int curr_attack;

  for (int i = 0; i < pok->numAttacks; i++) {
    if (pok->attacks[i].curr_pp) {
      available_attacks[num_available_attacks] = i;
      num_available_attacks++;
    }
  }
  if (num_available_attacks == 0) return -1;

  int max_move_index = available_attacks[0];

  //Get move with the maximum power.
  for (int i = 0; i < num_available_attacks; i++) {
    curr_attack = available_attacks[i];
    if (pok->attacks[curr_attack].power > pok->attacks[max_move_index].power) {
      max_move_index = available_attacks[i];
    }
  }

  int random = rand() % 100;
  if (random < 67) return max_move_index;
  else return available_attacks[rand() % num_available_attacks];
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
    if (player_get_num_alive() && !run_success && (currentPok->level < 100)) {
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