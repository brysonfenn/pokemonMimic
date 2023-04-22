#include "print_utils.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"

#include "conditions.h"

void clearTerminal() { printf("\033[2J\033[1;1H"); }

void printBattle(pokemon *pok1, pokemon *pok2) {
  printf("\t\t\t\t%s  Lvl %d  ", pok2->name, pok2->level);
  print_condition(pok2);
  printf("\n\t\t\t\tHP: %d/%d\n\n", pok2->currentHP, pok2->maxHP);
  printf("%s  Lvl %d  ", pok1->name, pok1->level);
  print_condition(pok1);
  printf("\nHP: %d/%d\n\n", pok1->currentHP, pok1->maxHP);
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

