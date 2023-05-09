#include "doors.h"

#include "location.h"

#define MAX_DOOR_COUNT 60

static Location doors[MAX_DOOR_COUNT];
static int door_count = 0;

static Location empty_door = {0,0,0,0,0,0,0};

//Add a door at a given location associated with an action
void add_door(char x, char y, short action, bool vertical) {
    doors[door_count].x = x;
    doors[door_count].y = y;
    doors[door_count].action = action;
    door_count++;
    if (vertical) {
        doors[door_count].x = x+1;
        doors[door_count].y = y;
        doors[door_count].action = action;
        door_count++;
    }
}

//Add a door to another map
    // x and y are the location of the door
    // next_* variables are where the door leads
void add_portal(char x, char y, char next_x, char next_y, char next_map, bool vertical) {
    doors[door_count].x = x;
    doors[door_count].y = y;
    doors[door_count].action = -1;
    doors[door_count].next_x = next_x;
    doors[door_count].next_y = next_y;
    doors[door_count].next_map = next_map;
    door_count++;

    if (vertical)
        add_portal(x+1, y, next_x+1, next_y, next_map, false);
}

// Return a door (if there is one) at player location, else return zero-door
struct Location * get_door(int player_x, int player_y) {
    for (int i = 0; i < door_count; i++) {
        if (player_x == doors[i].x && player_y == doors[i].y) {
            return &(doors[i]);
        }
    }
    return &(empty_door);
}

//Erase door list (called when changing or resetting map)
void clear_doors() {
    door_count = 0;
}