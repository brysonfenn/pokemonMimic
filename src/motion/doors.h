#ifndef DOORS_H
#define DOORS_H

#include <stdbool.h>

//Should be followed by: add_portal() or add_building_portal() depending on if this leads to a building interior
#define DOORS_NEW_MAP -1

struct Location;

//Add a door at a given location associated with an action
void add_door(char x, char y, short action, bool vertical);

//Add a door to another map
    // x and y are the location of the door
    // next_* variables are where the door leads
void add_portal(char x, char y, char next_x, char next_y, char next_map, bool vertical);

//Auto add portal to a building
void add_building_portal(char building_x, char building_y, char next_map);

//Auto add exit
void add_exit_portal(int side, int position, int map);

// Return a door (if there is one) at player location, else return zero-door
struct Location * get_door(int player_x, int player_y);

//Erase door list (called when changing or resetting map)
void clear_doors();

#endif //DOORS_H