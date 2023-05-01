#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#define INVALID_INPUT -9

void clearTerminal();

void printBattle();

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines);

int getValidInput(int beginRange, int endRange, const char * request);

void clearLastLine();

void resume_ncurses();
void pause_ncurses();

#endif // PRINT_UTILS_H