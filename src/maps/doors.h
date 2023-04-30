#ifndef DOORS_H
#define DOORS_H

void add_door(char x, char y, short action);

int get_door_action(int player_x, int player_y);

int get_door_count();

void clear_doors();

#endif //DOORS_H