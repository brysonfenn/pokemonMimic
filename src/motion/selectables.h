#ifndef SELECTABLES_H
#define SELECTABLES_H

#include <stdbool.h>

typedef struct Selectable {
    int x;
    int y;
    bool is_trainer;
    void * data;
} Selectable;


struct Trainer;

//Add a trainer at a given location
void add_trainer(char x, char y, struct Trainer * trainer, char face_direction);

//Add a trainer at a given location
void add_trainer_by_id(char x, char y, int trainer_id, char face_direction);

//Add message (sign, notification, etc)
void add_selectable_message(char x, char y, char * message);

// Return a selectable
Selectable * get_selectable(int player_x, int player_y, char player_char);

//Erase selectable list
void clear_selectables();

//Return a selectable containing a trainer if it is within battle distance of the player
Selectable * get_triggered_selectable(int player_x, int player_y, int *x_inc, int *y_inc);

#endif //SELECTABLES_H