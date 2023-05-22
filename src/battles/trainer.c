#include "trainer.h"

#include <stdio.h>

#include "battle.h"
#include "../monsters/pokemon.h"
#include "../player.h"
#include "../print_defines.h"
#include "../print_utils.h"

#define NAME_MAX_LENGTH 20

static char name[NAME_MAX_LENGTH];

//Battle a trainer with a random name and random # of pokemon
int battle_trainer() {
  char print_str[1024];

  if (!player_get_num_alive()) {
    begin_list();
    print_to_list("  All Pokemon have fainted, please heal them.\n");
    sleep(3);
    return BATTLE_WHITE_OUT;
  }

  int num_trainer_pokemon = (rand() % 3) + 2;
  int inputNum, battle_result, return_execute;
  pokemon trainer_pokemon[num_trainer_pokemon];
  char * trainer_name;
  battle_result = BATTLE_WIN;
  player.trainer_battle = true;

  trainer_name = get_random_name();

  begin_list();

  sprintf(print_str, "  Trainer %s wants to fight!\n", trainer_name);
  print_to_list(print_str); sleep(2);
  
  begin_list();
  sprintf(print_str, "  Trainer %s Pokemon: ", trainer_name);

  for (int i = 0; i < num_trainer_pokemon; i++) {
    trainer_pokemon[i] = *(get_random_pokemon(3,7));
    // trainer_pokemon[i] = *get_random_pokemon(30,33);
    sprintf(print_str, "%s*", print_str);
  }
  sprintf(print_str, "%s\n", print_str); 

  print_to_list(print_str); sleep(2);

  set_current_pokemon(PLAYER_DEFAULT_POKEMON);
  sprintf(print_str, "  B sent out %s\n", player.current_pokemon->name);
  print_to_list(print_str); sleep(2);

  int last_pokemon_pos = num_trainer_pokemon - 1;

  for (int i = 0; i < last_pokemon_pos; i++) {
    begin_list();
    sprintf(print_str, "  Trainer %s sent out %s\n", trainer_name, trainer_pokemon[i].name);
    print_to_list(print_str); sleep(2);
    battle_result = initiate_battle(&(trainer_pokemon[i]));

    if (battle_result == BATTLE_WHITE_OUT) { break; }

    //If the player has more than one pokemon, they should be able to switch now
    if (player_get_num_alive() > 1) {
      begin_list();
      sprintf(print_str, "  Trainer %s is about to send out %s\n", trainer_name, trainer_pokemon[i+1].name);
      sprintf(print_str, "%s  Will B change Pokemon?\n  Yes\n  No\n", print_str);
      print_to_list(print_str);
      inputNum = get_selection(LIST_BOX_Y+3,0,1,0);

      //Get player input for pokemon
      if (inputNum == 0) {
        while (1) {
          begin_list();
          print_to_list("Select a pokemon to use.\n");
          printParty();
        
          inputNum = get_selection(LIST_BOX_Y+3,0,player.numInParty-1,0);

          if (inputNum == PRESSED_B) {
            break;
          }
          else if (player.party[inputNum].currentHP == 0) {
            move(player.numInParty+3,0);
            print_to_list(" \nYou must select a different pokemon.\n"); sleep(2);
          }
          else {
            set_current_pokemon(inputNum);
            move(player.numInParty+3,0);
            sprintf(print_str, "B sent out %s\n", player.current_pokemon->name);
            print_to_list(print_str); sleep(2);
            break;
          }
        }
      }
    }
  }

  //Handle last pokemon
  if (battle_result == BATTLE_WIN) {
    begin_list();
    sprintf(print_str, "  Trainer %s sent out %s\n", trainer_name, trainer_pokemon[last_pokemon_pos].name);
    print_to_list(print_str); sleep(2);
    battle_result = initiate_battle(&(trainer_pokemon[last_pokemon_pos]));
    
    if (battle_result == BATTLE_WHITE_OUT) return BATTLE_WHITE_OUT;

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("B defeated Trainer %s", trainer_name);
    refresh(); sleep(2);
    int money_earned = (num_trainer_pokemon * 200) - 100;

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("B gained $%d for defeating Trainer %s", money_earned, trainer_name);
    player.money += money_earned;
    refresh(); sleep(2);
  }
  
  if (battle_result == BATTLE_WHITE_OUT) {
    return BATTLE_WHITE_OUT;
  }
  else {
    for (int i = 0; i < player.numInParty; i++) {
      reset_stat_stages(&(player.party[i]));
    }
    return BATTLE_WIN;
  }
}

//Get a random name (could be female or male)
char * get_random_name() {
  FILE *og_file;
  // Open the file for reading
  og_file = fopen("names.txt", "r");
  char line[50];

  // Check if the file was opened successfully
  if (og_file == NULL) {
      printw("File does not exist.\n"); refresh(); sleep(2);
      return 1;
  }

  int name_pos = rand() % 2781;
  int count = 0;

  while (count < name_pos) {
    fgets(line, NAME_MAX_LENGTH, og_file);
    sscanf(line, "%s", name);
    count++;
  }

  return name;
}