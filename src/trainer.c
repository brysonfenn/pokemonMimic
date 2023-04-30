#include "trainer.h"

#include <stdio.h>

#include "battle.h"
#include "pokemon.h"
#include "player.h"

#define NAME_MAX_LENGTH 20

static char name[NAME_MAX_LENGTH];

int battleTrainer() {
  int num_trainer_pokemon = (rand() % 3) + 2;
  int inputNum, battle_result, return_execute;
  pokemon trainer_pokemon[num_trainer_pokemon];
  char * trainer_name;
  battle_result = BATTLE_WIN;
  player.trainer_battle = true;

  trainer_name = get_random_name();

  printf("Trainer %s wants to fight!\n", trainer_name); sleep(2);
  clearTerminal();
  printf("Trainer %s Pokemon: ", trainer_name);;

  for (int i = 0; i < num_trainer_pokemon; i++) {
    trainer_pokemon[i] = *(get_random_pokemon(3,6));
    printf("*");
  }
  printf("\n"); sleep(2);

  set_current_pokemon(PLAYER_DEFAULT_POKEMON);
  printf("B sent out %s\n", player.current_pokemon->name); sleep(2);

  int last_pokemon_pos = num_trainer_pokemon - 1;

  for (int i = 0; i < last_pokemon_pos; i++) {
    printf("Trainer %s sent out %s\n", trainer_name, trainer_pokemon[i].name);
    sleep(2);
    battle_result = initiate_battle(trainer_pokemon[i]);

    if (battle_result == BATTLE_WHITE_OUT) { break; }

    clearTerminal();
    printf("Trainer %s is about to send out %s\n", trainer_name, trainer_pokemon[i+1].name);
    inputNum = getValidInput(0,1, "Will B change Pokemon?\n1: Yes\t0:No\n");

    //Get player input for pokemon
    if (inputNum == 1) {
      clearTerminal();
      printParty();
      printf("\n");
      while (1) {
        inputNum = getValidInput(1, player.numInParty, "Select a Pokemon to use: ");
        inputNum--; //Adjust inputNum to array position
        if (player.party[inputNum].currentHP == 0) {
          printf("You must select a different pokemon.\n"); sleep(2);
          clearLastLine(); clearLastLine();
        }
        else {
          set_current_pokemon(inputNum);
          printf("B sent out %s\n", player.current_pokemon->name); sleep(2);
          break;
        }
      }
    }
  }

  if (battle_result == BATTLE_WIN) {
    printf("Trainer %s sent out %s\n", trainer_name, trainer_pokemon[last_pokemon_pos].name);
    sleep(2);
    battle_result = initiate_battle(trainer_pokemon[last_pokemon_pos]);
    printf("B defeated Trainer %s\n", trainer_name);
    sleep(2);
    int money_earned = (num_trainer_pokemon * 200) - 100;
    printf("B gained $%d for defeating Trainer %s\n", money_earned, trainer_name);
    player.money += money_earned;
    sleep(2);
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
      printf("File does not exist.\n"); sleep(2);
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