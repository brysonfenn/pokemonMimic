#include "trainer.h"

#include <stdio.h>

#include "battle.h"
#include "../monsters/pokemon.h"
#include "../player.h"

#define NAME_MAX_LENGTH 20

static char name[NAME_MAX_LENGTH];

int battle_trainer() {

  if (!player.numAlive) {
    printw("  All Pokemon have fainted, please heal them.\n");
    refresh(); sleep(3);
    return BATTLE_WHITE_OUT;
  }

  int num_trainer_pokemon = (rand() % 3) + 2;
  int inputNum, battle_result, return_execute;
  pokemon trainer_pokemon[num_trainer_pokemon];
  char * trainer_name;
  battle_result = BATTLE_WIN;
  player.trainer_battle = true;

  trainer_name = get_random_name();

  printw("  Trainer %s wants to fight!\n", trainer_name); refresh(); sleep(2);
  clear();
  printw("  Trainer %s Pokemon: ", trainer_name);

  for (int i = 0; i < num_trainer_pokemon; i++) {
    trainer_pokemon[i] = *(get_random_pokemon(3,7));
    // trainer_pokemon[i] = *get_random_pokemon(30,33);
    printw("*");
  }
  printw("\n"); refresh(); sleep(2);

  set_current_pokemon(PLAYER_DEFAULT_POKEMON);
  printw("  B sent out %s\n", player.current_pokemon->name); refresh(); sleep(2);

  int last_pokemon_pos = num_trainer_pokemon - 1;

  for (int i = 0; i < last_pokemon_pos; i++) {
    clear();
    mvprintw(0,0,"  Trainer %s sent out %s\n", trainer_name, trainer_pokemon[i].name);
    refresh(); sleep(2);
    battle_result = initiate_battle(trainer_pokemon[i]);

    if (battle_result == BATTLE_WHITE_OUT) { break; }

    clear();
    printw("  Trainer %s is about to send out %s\n", trainer_name, trainer_pokemon[i+1].name);
    printw("  Will B change Pokemon?\n  Yes\n  No\n");
    inputNum = get_selection(2,0,1,0);

    //Get player input for pokemon
    while (1) {
      if (inputNum == 0) {
        clear();
        printw("Select a Pokemon to use:\n");
        printParty();
        printw("\n");
      
        inputNum = get_selection(2,0,player.numInParty-1,0);

        if (player.party[inputNum].currentHP == 0) {
          move(player.numInParty+3,0);
          printw("You must select a different pokemon.\n"); refresh(); sleep(2);
        }
        else {
          set_current_pokemon(inputNum);
          move(player.numInParty+3,0);
          printw("B sent out %s\n", player.current_pokemon->name); refresh(); sleep(2);
          break;
        }
      }
      else {
        break;
      }
    }
  }

  if (battle_result == BATTLE_WIN) {
    printw("Trainer %s sent out %s\n", trainer_name, trainer_pokemon[last_pokemon_pos].name);
    refresh(); sleep(2);
    battle_result = initiate_battle(trainer_pokemon[last_pokemon_pos]);
    
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
    return BATTLE_WIN;
  }
}


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