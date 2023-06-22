#include "item_funcs.h"

#include "items.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"
#include "../monsters/pokemon.h"
#include "../player.h"


//Potion execute function
int execute_potion(int hp_gain, char * name) {
  char print_str[4096];

  begin_list();
  sprintf(print_str, "Select a pokemon to apply the %s on.\n", name);
  print_to_list(print_str);
  printParty();
  print_to_list("  Cancel\n");

  int input = get_selection(2, player.numInParty, 0);
  if (input == player.numInParty || input == PRESSED_B) return ITEM_FAILURE;

  int currentHP = player.party[input].currentHP;

  if (currentHP == 0 || currentHP == player.party[input].maxHP) {
    sprintf(print_str, " \nCould not apply %s on %s.\n", name, player.party[input].name);
    print_to_list(print_str); sleep(2);
    return ITEM_FAILURE;
  }
  int tempHP = player.party[input].currentHP;

  player.party[input].currentHP += hp_gain;
  if (player.party[input].currentHP > player.party[input].maxHP) {
    player.party[input].currentHP = player.party[input].maxHP;
  }

  sprintf(print_str, " \n%s gained %d HP!\n", player.party[input].name, player.party[input].currentHP - tempHP);
  print_to_list(print_str); sleep(2);

  begin_list();
  printParty(); refresh(); sleep(2);

  return ITEM_SUCCESS;
}


//Pokeball execute function (catch_rate is % multiplier)
int attempt_catch(int catch_rate, char * name) {
    Pokemon * enemy_pok = player.enemy_pokemon;

    if (!player.is_battle || player.trainer_battle) {
        begin_list();
        print_to_list("You can't use that!"); sleep(2);
        return ITEM_FAILURE;
    }
    else if (player.numInParty >= 6 && player.numInPCStorage >= 15) {
        begin_list();
        print_to_list("Your party and your PC storage are full!"); sleep(2);
        return ITEM_FAILURE;
    }

    //Catch rate calculations
    float ball_rate = (float) catch_rate;
    ball_rate = ball_rate / 100;
    catch_rate = pokemon_get_catch_rate(enemy_pok->id_num);
    float hp_rate = (3.0*enemy_pok->maxHP - 2.0*enemy_pok->currentHP) / (3.0*enemy_pok->maxHP);
    catch_rate = (int) (catch_rate * ball_rate * hp_rate);

    //Add condition rates
    Condition condition = enemy_pok->visible_condition;
    if (condition == PARALYZED || condition == POISONED || condition == BURNED) catch_rate *= 1.5;
    if (condition == ASLEEP || condition == FROZEN) catch_rate *= 2.0;
    
    if (catch_rate < 1) catch_rate = 1;

    int random = rand() % 256;
    clear();
    printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("%s threw a %s!", player.name, name); refresh(); sleep(2);

    text_box_cursors(TEXT_BOX_NEXT_LINE); //Set next line for successful or unsuccessful catch

    //Pokemon is only caught if
    if (random < catch_rate) {
        printw("%s was caught!", enemy_pok->name); refresh(); sleep(2);

        //If player already has 6 pokemon, transfer the new pokemon to the PC
        if (player.numInParty >= 6) {
        player.pc_storage[player.numInPCStorage] = (*enemy_pok);
        player.numInPCStorage++;

        //Reset all stats of that pokemon in the PC
        Pokemon * new_pok = &(player.pc_storage[player.numInPCStorage-1]);
        new_pok->currentHP = new_pok->maxHP;
        new_pok->visible_condition = NO_CONDITION;
        reset_stat_stages(new_pok);
        for (int i = 0; i < new_pok->numAttacks; i++) {
            new_pok->attacks[i].curr_pp = new_pok->attacks[i].max_pp;
        }

        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("%s was transferred to PC storage.", enemy_pok->name); refresh(); sleep(2);
        }
        //Add Pokemon to party
        else {
        player.party[player.numInParty] = (*enemy_pok);
        player.numInParty++;
        }
        return ITEM_CATCH_SUCCESS;
    }
    else {
        printw("Catch unsuccessful!"); refresh(); sleep(2);
        return ITEM_CATCH_FAILURE;
    }
}


//Heal Condition function (condition is condition code)
int heal_condition(int condition, char * name) {
    char print_str[1024];
    Pokemon * selected_pok;

    begin_list();
    sprintf(print_str, "Select a pokemon to apply the %s on.\n", name);
    print_to_list(print_str);
    printParty();
    print_to_list("  Cancel\n");

    int input = get_selection(2, player.numInParty, 0);
    if (input == player.numInParty || input == PRESSED_B) return ITEM_FAILURE;

    selected_pok = &(player.party[input]);

    if (selected_pok->currentHP == 0 || selected_pok->visible_condition != condition) {
        sprintf(print_str, " \nCould not apply %s on %s.\n", name, selected_pok->name);
        print_to_list(print_str); sleep(2);
        return ITEM_FAILURE;
    }
    
    selected_pok->visible_condition = NO_CONDITION;

    sprintf(print_str, " \n%s's condition was healed!\n", selected_pok->name);
    print_to_list(print_str); sleep(2);

    begin_list();
    printParty(); refresh(); sleep(2);

    return ITEM_SUCCESS;
}


//Revive a pokemon up to percent of their HP
int revive_pokemon(int percent, char * name) {
    char print_str[1024];
    Pokemon * selected_pok;

    begin_list();
    sprintf(print_str, "Select a pokemon to apply the %s on.\n", name);
    print_to_list(print_str);
    printParty();
    print_to_list("  Cancel\n");

    int input = get_selection(2, player.numInParty, 0);
    if (input == player.numInParty || input == PRESSED_B) return ITEM_FAILURE;

    selected_pok = &(player.party[input]);

    if (selected_pok->currentHP != 0) {
        sprintf(print_str, " \nCould not apply %s on %s.\n", name, selected_pok->name);
        print_to_list(print_str); sleep(2);
        return ITEM_FAILURE;
    }

    int half_hp = (int) (selected_pok->maxHP * (percent / 100.0)) + 1;
    
    selected_pok->currentHP = half_hp;

    sprintf(print_str, " \n%s was revived!\n", selected_pok->name);
    print_to_list(print_str); sleep(2);

    begin_list();
    printParty(); refresh(); sleep(2);

    return ITEM_SUCCESS;
}


//Filler function for empty_item that does nothing
int do_nothing(int input_num) {
  return ITEM_FAILURE;
}