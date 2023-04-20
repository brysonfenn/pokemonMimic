#include "print_utils.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "termios.h"

struct termios orig_termios, new_termios;

void ignoreInput() {
  tcgetattr(0, &orig_termios);
  new_termios = orig_termios;
  new_termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &new_termios);
}

void acceptInput() {
  tcsetattr(0, TCSANOW, &orig_termios);
}

void clearTerminal() { printf("\033[2J\033[1;1H"); }

void printBattle(pokemon *pok1, pokemon *pok2) {
  printf("\t\t\t\t%s  Lvl %d\n", pok2->name, pok2->level);
  printf("\t\t\t\tHP: %d/%d\n\n", pok2->currentHP, pok2->maxHP);
  printf("%s  Lvl %d\n", pok1->name, pok1->level);
  printf("HP: %d/%d\n\n", pok1->currentHP, pok1->maxHP);
}

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines) {
  int inputNum = INVALID_INPUT;
  bool valid = false;
  char input[100];
  
  while (!valid) {
    printf("%s", request);
    fgets(input, 100, stdin);
    sscanf(input, "%d", &inputNum);
    
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

