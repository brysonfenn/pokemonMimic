#ifndef SELECTABLES_H
#define SELECTABLES_H

#include <stdbool.h>

struct Trainer;

//Add a trainer at a given location
void add_trainer(char x, char y, struct Trainer * trainer, char face_direction);

//Add message (sign, notification, etc)
void add_selectable_message(char x, char y, char * message);

// Return a selectable
void * get_selectable(int player_x, int player_y, char player_char, bool * is_trainer);

//Erase selectable list
void clear_selectables();

#endif //SELECTABLES_H