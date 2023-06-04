#ifndef PRINT_TERMINAL_H
#define PRINT_TERMINAL_H

//THESE FUNCTIONS ARE CURRENTLY UNUSED FOR NORMAL GAMEPLAY IN FAVOR OF NCURSES FUNCTIONS

void clearTerminal();
void clearLastLine();

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines);
int getValidInput(int beginRange, int endRange, const char * request);

#endif //PRINT_TERMINAL_H