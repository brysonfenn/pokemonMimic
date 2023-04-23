#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#define INVALID_INPUT -9

#include "pokemon.h"

void clearTerminal();

void printBattle(pokemon *pok1, pokemon *pok2);

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines);

int getValidInput(int beginRange, int endRange, const char * request);

void clearLastLine();

#endif // PRINT_UTILS_H