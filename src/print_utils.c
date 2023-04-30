#include "print_utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <ncurses.h>

#include "conditions.h"
#include "player.h"
#include "pokemon.h"

void clearTerminal() { printf("\033[2J\033[1;1H"); }

void printBattle() {
  pokemon * player_pok = player.current_pokemon;
  pokemon * enemy_pok = player.enemy_pokemon;
  printf("\t\t\t\t%s  Lvl %d  ", enemy_pok->name, enemy_pok->level);
  print_condition(enemy_pok);
  printf("\n\t\t\t\tHP: %d/%d\n\n", enemy_pok->currentHP, enemy_pok->maxHP);
  printf("%s  Lvl %d  ", player_pok->name, player_pok->level);
  print_condition(player_pok);
  printf("\nHP: %d/%d\n\n", player_pok->currentHP, player_pok->maxHP);
}

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines) {
  int inputNum = INVALID_INPUT;
  bool valid = false;
  char input[100];
  char c;
  
  while (!valid) {
    printf("%s", request);
    fgets(input, 100, stdin);
    sscanf(input, "%d", &inputNum);

    //If the user entered a number greater than they should, get the lowest digit.
    if (inputNum > endRange) { inputNum = inputNum % 10; }
    
    if (inputNum < beginRange || inputNum > endRange) {
      if (input[0] == '\n') {
        clearLastLine();
        continue;
      }
      sleep(1);
      printf("Please enter a valid number between %d and %d.\n", beginRange, endRange);
      sleep(1);
      for (int i = 0; i < erase_lines; i++) clearLastLine();
      inputNum = INVALID_INPUT;
    }
    else {
      valid = true;
    }
  }
  
  return inputNum;
}

int getValidInput(int beginRange, int endRange, const char * request) {
  return getValidInput_force(beginRange, endRange, request, 2);
}

void clearLastLine() {
  printf("\033[1A"); // Move cursor up one line
  printf("\033[K"); // Clear the line
}
