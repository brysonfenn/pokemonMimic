#ifndef PRINT_TERMINAL_H
#define PRINT_TERMINAL_H

void clearTerminal();
void clearLastLine();

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines);
int getValidInput(int beginRange, int endRange, const char * request);

#endif //PRINT_TERMINAL_H