#include "trainer.h"

#include "battle.h"
#include "pokemon.h"
#include "player.h"

int battleTrainer() {
  int num_trainer_pokemon = 3;
  int inputNum, battle_result, return_execute;
  pokemon trainer_pokemon[num_trainer_pokemon];
  char trainer_name[50] = "G";
  battle_result = WIN;

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

    if (battle_result == WHITE_OUT) { break; }

    clearTerminal();
    printf("Trainer %s is about to send out %s\n", trainer_name, trainer_pokemon[i+1].name);
    inputNum = getValidInput(0,1, "Will B change Pokemon?\n0: Yes\t1:No\n");

    //Get player input for pokemon
    if (!inputNum) {
      clearTerminal();
      printParty();
      printf("\n");
      while (1) {
        inputNum = getValidInput(0, player.numInParty - 1, "Select a Pokemon to use: ");
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

  if (battle_result == WIN) {
    printf("Trainer %s sent out %s\n", trainer_name, trainer_pokemon[last_pokemon_pos].name);
    sleep(2);
    battle_result = initiate_battle(trainer_pokemon[last_pokemon_pos]);
    printf("B defeated Trainer %s\n", trainer_name);
    sleep(2);
    printf("B gained $%d for defeating Trainer %s\n", num_trainer_pokemon * 100, trainer_name);
    player.money += num_trainer_pokemon * 100;
    sleep(2);
  }
  
  if (battle_result == WHITE_OUT) {
    return WHITE_OUT;
  }
  else {
    return WIN;
  }
}