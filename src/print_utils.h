#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#define INVALID_INPUT -9

#define TEXT_BOX_BEGINNING 0
#define TEXT_BOX_NEXT_LINE 1

void clearTerminal();
void clearLastLine();

void printBattle();

int getValidInput_force(int beginRange, int endRange, const char * request, int erase_lines);

int getValidInput(int beginRange, int endRange, const char * request);

void printParty();
void printBag();

void resume_ncurses();
void pause_ncurses();

int get_selection(int first_line, int start, int end, int last_selection);
int get_battle_selection(int first_line, int last_selection);
int get_fight_selection(int first_line, int num_attacks);

void drawBox(int x, int y, int w, int h);

void draw_text_box();
void text_box_cursors(int next_line);
void clear_text_box();

void draw_battle_box();

#endif // PRINT_UTILS_H