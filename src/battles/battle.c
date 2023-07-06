#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "ncurses.h"

#include "battle.h"
#include "../player.h"
#include "../monsters/conditions.h"
#include "../monsters/pokemon.h"

#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../print/print_battle.h"
#include "../items/items.h"
#include "../motion/motion2d.h"
#include "../motion/maps.h"

static enum display { MAIN, FIGHT, BAG, POKEMON } current_display = MAIN;
static enum decision {NONE, ATTACK, ITEM, SWITCH, RUN } current_decision = NONE;

static bool pokemon_needing_exp[6] = {false, false, false, false, false, false};

//Forward Declarations
void perform_enemy_attack(Pokemon * currentPok, Pokemon * enemyPok, int attack_num);
int get_enemy_move(Pokemon * pok);
void handle_exp(int exp);
bool run_attempt();

//Begin a Battle with a given pokemon
int handle_battle(struct Pokemon * enemyPok) {
  int inputNum, max_input;
  bool enemy_attacks, fainted_switch, out_of_pp, run_success, catch_success;
  int return_execute, attack_result, attack_num, enemy_attack_num, item_num, pokemon_selected, speed_difference;
  char print_str[512];

  attack_num = item_num = pokemon_selected = speed_difference = 0;
  enemy_attacks = false;
  fainted_switch = false;
  run_success = false;
  catch_success = false;

  Pokemon *currentPok = player.current_pokemon;
  player.enemy_pokemon = enemyPok;
  player.is_battle = true;

  //First used pokemon needs exp, no one else does for now
  for (int i = 0; i < player.numInParty; i++) {
    if (currentPok == &(player.party[i])) {
      pokemon_needing_exp[i] = true;
    }
    else {
      pokemon_needing_exp[i] = false;
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
      remove_hidden_condition(currentPok, REPEAT_MOVE);
      printw("%s fainted. ", currentPok->name); refresh(); sleep(2);
      
      // Handle White out
      if (player_get_num_alive() == 0) {
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("....."); refresh(); sleep(2);
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("%s is out of usable pokemon... ", player.name); refresh(); sleep(2);
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("%s whited out.", player.name); refresh(); sleep(2);

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

    //Handle Repeating Move
    if (has_hidden_condition(currentPok, REPEAT_MOVE) && currentPok->last_move != NO_LAST_MOVE) {
      current_decision = ATTACK;
      attack_num = currentPok->last_move;
    }
    else {
      current_decision = NONE;
    }
    
    int last_selection = 0;

    ///////// GET DECISION /////////
    while (current_decision == NONE) {
      clear();

      enemy_attack_num = get_enemy_move(enemyPok);

      switch (current_display) {
        
      case MAIN:
        printBattle();
        mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  Fight"); mvprintw(SELECT_Y,BATTLE_SELECT_2_X,"  Bag");
        mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  Pokémon"); mvprintw(SELECT_Y+1,BATTLE_SELECT_2_X,"  Run");
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("What will %s do?", player.name);

        inputNum = get_battle_selection(SELECT_Y, last_selection);
        last_selection = inputNum;
        run_success = false;
        if (inputNum == 3) {
          current_decision = RUN;
          break;
        }
        current_display = inputNum+1; //In this case, inputNum + 1 will be the enum position
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
          attack_num = STRUGGLE_MOVE_NUM;
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
        if (inputNum == 4 || inputNum == PRESSED_B) {
          current_display = MAIN;
          break;
        }

        attack_num = inputNum;
        current_decision = ATTACK;
        break;
        
      case BAG:
        begin_list();
        printBag();
        inputNum = get_selection(1, player.numInBag, 0);
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
        inputNum = get_selection(2, max_input, 0);

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

    printBattle();

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
        attack_result = perform_attack(currentPok, attack_num, enemyPok, false);
        if (attack_result == 0) enemy_attacks = true;
      }
      else if (speed_difference < 0) {
        perform_enemy_attack(currentPok, enemyPok, enemy_attack_num);
        clear();
        printBattle();
        clear_text_box();
        //Player Pokemon can only attack if still alive
        if (currentPok->currentHP > 0 && enemyPok->currentHP > 0) {
          attack_result = perform_attack(currentPok, attack_num, enemyPok, false);
        }
      }

      current_display = MAIN;
      break;
    case ITEM:
      return_execute = use_item(inputNum);
      //Return to bag menu if item failed, else back to main menu
      if (return_execute == ITEM_FAILURE) { continue; }
      else if (return_execute == ITEM_CATCH_SUCCESS) { catch_success = true; }
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
        text_box_cursors(TEXT_BOX_BEGINNING);

        set_current_pokemon(pokemon_selected);
        currentPok = player.current_pokemon;
        enemy_attacks = true;

        //Advise the player of the switch
        clear();
        printBattle();
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("%s sent out %s!", player.name, player.current_pokemon->name); refresh(); sleep(2);

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
        run_success = run_attempt();
        enemy_attacks = true;
      }
      break;
    default:
      break;
    }
    /////// END EXECUTE DECISION ///////

    if (run_success || catch_success) break;  //Handle Run Away

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

    handle_end_conditions(enemyPok);
    handle_end_conditions(currentPok);
  }

  if (!run_success && !catch_success) {
    int exp = pokemon_get_exp_yield(enemyPok);
    float random = (float) (rand() % 25);
    exp *= (1.0 + (random / 100.0));
    if (player.trainer_battle) exp *= 1.5;
    handle_exp(exp);
  }

  remove_all_hidden_conditions(enemyPok);
  
  if (catch_success)
    return BATTLE_CAUGHT_POKE;
  else
    return BATTLE_WIN;
}


//Handle an enemyPok attacking the player's current pokemon
void perform_enemy_attack(Pokemon * currentPok, Pokemon * enemyPok, int attack_num) {
  clear();
  printBattle();
  perform_attack(enemyPok, attack_num, currentPok, true);

  if (currentPok->currentHP <= 0) {
    currentPok->currentHP = 0;
    clear();
    printBattle();
  }
  if (enemyPok->currentHP <= 0) {
    enemyPok->currentHP = 0;
    clear();
    printBattle();
  }
}


//Get a move number randomly (move with highest damage has a higher chance)
int get_enemy_move(Pokemon * pok) {
  int num_available_attacks = 0;
  int available_attacks[4];
  int curr_attack;

  //Create an array of moves that have remaining PP > 0
  for (int i = 0; i < pok->numAttacks; i++) {
    if (pok->attacks[i].curr_pp && (get_hidden_condition_val(pok, DISABLED) != i)) {
      available_attacks[num_available_attacks] = i;
      num_available_attacks++;
    }
  }

  //Enemy should struggle if no moves have PP
  if (num_available_attacks == 0) return STRUGGLE_MOVE_NUM;

  //Varables to assist in getting move with maximum damage
  int max_move_index = available_attacks[0];
  int max_damage = 0;
  int curr_damage = 0;
  int flags_var;  //Filler int for the get_damage function

  //Get move with maximum damage
  for (int i = 0; i < num_available_attacks; i++) {
    curr_attack = available_attacks[i];
    curr_damage = get_damage(pok, curr_attack, player.current_pokemon, false, &flags_var);
    if (curr_damage > max_damage) {
      max_move_index = available_attacks[i];
      max_damage = curr_damage;
    }
  }

  // Higher chance of selecting move with highest damage
  int random = rand() % 100;
  if (random < 67) return max_move_index;
  else return available_attacks[rand() % num_available_attacks];
}


//Give all pokemon exp that need it, and level up
void handle_exp(int exp) {
  Pokemon * currentPok;
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
    //Pokemon at level 100 should not gain experience.
    if (player_get_num_alive() && (currentPok->level < 100)) {
      text_box_cursors(TEXT_BOX_NEXT_LINE);
      printw("%s gained %d experience points!", currentPok->name, exp);
      currentPok->exp += (exp);
      refresh(); sleep(2);
    }

    next_level_exp = pokemon_get_next_level_exp(currentPok);

    //Update levels
    while (currentPok->exp >= next_level_exp) {
      pokemon_level_up(currentPok, next_level_exp);
      next_level_exp = pokemon_get_next_level_exp(currentPok);
    }
  }
  
  for (int i = 0; i < 6; i++) { pokemon_needing_exp[i] = false; }
}


//Try to run away
bool run_attempt() {
  int random = (rand() % 256);
  int chance = (player.current_pokemon->baseSpeed * 128 / player.enemy_pokemon->baseSpeed);
  if (chance < 5) chance = 5;

  clear_text_box();
  text_box_cursors(TEXT_BOX_BEGINNING);
  if (random < chance) {
    printw("Got away safely."); refresh(); sleep(2);
    return true;
  }
  else {
    printw("Can't escape!"); refresh(); sleep(2);
    return false;
  }
}