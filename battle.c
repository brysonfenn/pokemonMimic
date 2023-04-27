#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "battle.h"
#include "player.h"
#include "conditions.h"

#include "print_utils.h"

static enum display { MAIN, FIGHT, BAG, POKEMON } current_display = MAIN;
static enum decision {NONE, ATTACK, ITEM, SWITCH, RUN } current_decision = NONE;

static bool pokemon_needing_exp[6] = {false, false, false, false, false, false};

static bool run_success;

void perform_enemy_attack(pokemon * currentPok, pokemon * enemy, int attack_num);

int get_move(pokemon * pok);

void handle_exp(int exp);

//Begin a Battle
int initiate_battle(pokemon enemyPoke) {
  int inputNum, max_input;
  bool enemy_attacks, fainted_switch;
  int return_execute, attack_num, enemy_attack_num, item_num, pokemon_selected, speed_difference;
  attack_num = item_num = pokemon_selected = speed_difference = 0;
  enemy_attacks = false;
  fainted_switch = false;
  run_success = false;

  pokemon enemy = enemyPoke;
  pokemon *currentPok = player.current_pokemon;
  player.enemy_pokemon = &enemy;

  //First used pokemon needs exp
  for (int i = 0; i < player.numInParty; i++) {
    if (currentPok == &(player.party[i])) {
      pokemon_needing_exp[i] = true;
    }
  }

  while (1) {
    clearTerminal();

    // Allow loop to break if enemyHP is 0 or less
    if (enemy.currentHP <= 0) {
      enemy.currentHP = 0; clearTerminal();
      printBattle(); sleep(2);
      printf("Enemy %s fainted.\n", enemy.name); sleep(2);
      break;
    }

    // If current pokemon faints, select a different pokemon.
    if (currentPok->currentHP == 0) {
      sleep(2);
      printf("%s fainted. \n", currentPok->name);
      // Handle White out
      if (player.numAlive == 0) {
        sleep(2);
        printf("\nB is out of usable pokemon...\n"); sleep(3);
        printf("B whited out.\n"); sleep(5);
        return WHITE_OUT;
      }
      printf("You must select a different pokemon.\n"); sleep(2);
      clearTerminal();
      fainted_switch = true;       //Do not allow an attack for this switch
      current_display = POKEMON;  //Force a Switch
    }

    inputNum = 999;

    current_decision = NONE;

    ///////// GET DECISION /////////
    while (current_decision == NONE) {
      clearTerminal();

      enemy_attack_num = get_move(&enemy);

      switch (current_display) {
        
      case MAIN:
        printBattle();
        printf("0: Fight   \t\t1: Bag     \n2: Pokemon\t\t3: Run\n\n");
        inputNum = getValidInput(0, 3, "What will B do? Select an Option: ");
        run_success = false;
        if (inputNum == 3) {
          current_decision = RUN;
          break;
        }
        current_display = inputNum + 1;
        enemy_attacks = false;
        break;
        
      case FIGHT:
        printBattle();
        printf("0: %s\r\t\t\t1: %s\n2: %s\r\t\t\t3: %s\r\t\t\t\t\t\t4: Cancel\n\n", currentPok->attacks[0].name,
               currentPok->attacks[1].name, currentPok->attacks[2].name,
               currentPok->attacks[3].name);
        inputNum = getValidInput(0, 4, "Select an Attack: ");
        //Handle Cancel
        if (inputNum == 4) {
          current_display = MAIN;
          break;
        }
        if (inputNum >= currentPok->numAttacks) inputNum = INVALID_INPUT;
        if (inputNum == INVALID_INPUT) {
          sleep(1);
          printf("Please enter a valid number between 0 and %d, or 4.\n", currentPok->numAttacks-1);
          sleep(1);
          continue; // Repeat While loop if input is invalid
        }
        attack_num = inputNum;
        current_decision = ATTACK;
        current_display = MAIN;
        break;
        
      case BAG:
        printBag();
        inputNum = getValidInput(0, player.numInBag, "Select an item to use: ");
        if (inputNum == player.numInBag) {
          current_display = MAIN;
          break;
        }
        item_num = inputNum;
        current_decision = ITEM;
        current_display = MAIN;
        break;
        
      case POKEMON:
        max_input = player.numInParty - 1;
        printParty();
        if (player.current_pokemon->currentHP != 0) {
          max_input++;
          printf("%d: Cancel\n", player.numInParty);
        }
        printf("\n");
        inputNum = getValidInput(0, max_input, "Select a Pokemon to use: ");
        if (inputNum == player.numInParty) {
          current_display = MAIN;
          continue;
        }
        if (player.party[inputNum].currentHP == 0) {
          printf("%s can't fight anymore! Select a different pokemon.\n", player.party[inputNum].name);
          sleep(2);
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
        clearTerminal();
        printBattle();
        //Player Pokemon can only attack if still alive
        if (currentPok->currentHP > 0) {
          printf("\n\n\n\n");
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

        reset_base_stats(currentPok);
        set_current_pokemon(pokemon_selected);
        currentPok = player.current_pokemon;
        enemy_attacks = true;

        // New pokemon needs exp
        pokemon_needing_exp[pokemon_selected] = true;
      }
      break;
    case RUN:
      if (player.trainer_battle) {
        printf("You can't run from a trainer battle!\n"); sleep(2);
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
      enemy.currentHP = 0; clearTerminal();
      printBattle(); sleep(2);
      printf("Enemy %s fainted.\n", enemy.name); sleep(2);
      break;
    }

    // Enemy performs attack if we just did something
    if (enemy_attacks && !fainted_switch) {
      perform_enemy_attack(currentPok, &enemy, enemy_attack_num);
    }
    fainted_switch = false;

    handle_end_conditions();

    clearTerminal();
  }

  handle_exp(enemy.level * 3);

  return WIN;
}


// Handle an enemy attack
void perform_enemy_attack(pokemon * currentPok, pokemon * enemy, int attack_num) {
  clearTerminal();
  printBattle();

  perform_attack(enemy, attack_num, currentPok, true);
  if (currentPok->currentHP <= 0) {
    currentPok->currentHP = 0;
    player.numAlive--;
    clearTerminal();
    printBattle();
  }
}


// Return a move number for a given pokemon
// Moves with higher power are more likely
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


//Give EXP to proper pokemon
void handle_exp(int exp) {
  pokemon * currentPok;
  int next_level_exp;
  int num_exp_earned_pokemon = 0;

  for (int i = 0; i < 6; i++) { 
    if (pokemon_needing_exp[i]) num_exp_earned_pokemon++;
  }

  if (num_exp_earned_pokemon == 0) { printf("Problem in handle_exp()!\n"); sleep(2); return; }

  exp = (exp / num_exp_earned_pokemon) + 1;

  for (int i = 0; i < 6; i++) {
    if (pokemon_needing_exp[i] == false) continue;

    currentPok = &(player.party[i]);

    //Give active pokemon experience points if it is alive and didn't run away.
    if (player.numAlive && !run_success) {
      printf("%s gained %d experience points!\n", currentPok->name, exp);
      currentPok->exp += (exp);
      sleep(2);
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