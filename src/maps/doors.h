#ifndef DOORS_H
#define DOORS_H

struct Location;

void add_door(char x, char y, short action);
void add_portal(char x, char y, char next_x, char next_y, char next_map);

struct Location * get_door(int player_x, int player_y);

int get_door_count();

void clear_doors();

#endif //DOORS_H