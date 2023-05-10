#include "doors.h"

#include "location.h"
#include "map_drawing.h"

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

void add_exit_portal(int side, int position, int map) {
    int min_x = MAP_X + position;
    int min_y = MAP_Y + position;
    int max_x = TOWN_WIDTH + MAP_X - 1;
    int max_y = TOWN_HEIGHT + MAP_Y - 1;

    switch (side) {
        case MAP_TOP:
            add_portal(min_x, MAP_Y, min_x, max_y-1, map, true);
            break;
        case MAP_BOTTOM:
            add_portal(min_x, max_y, min_x, MAP_Y+1, map, true);
            break;
        case MAP_RIGHT:
            add_portal(max_x, min_y, MAP_X+1, min_y, map, false);
            break;
        case MAP_LEFT:
            add_portal(MAP_X, min_y, max_x-1, min_y, map, false);
            break;
    }
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