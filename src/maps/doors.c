#include "doors.h"

#include "location.h"

#define MAX_DOOR_COUNT 30

static Location doors[MAX_DOOR_COUNT];
static int door_count = 0;

static Location empty_door = {0,0,0,0,0,0,0};

void add_door(char x, char y, short action) {
    doors[door_count].x = x;
    doors[door_count].y = y;
    doors[door_count].action = action;
    door_count++;
}

void add_portal(char x, char y, char next_x, char next_y, char next_map) {
    doors[door_count].x = x;
    doors[door_count].y = y;
    doors[door_count].action = -1;
    doors[door_count].next_x = next_x;
    doors[door_count].next_y = next_y;
    doors[door_count].next_map = next_map;
    door_count++;
}

struct Location * get_door(int player_x, int player_y) {
    for (int i = 0; i < door_count; i++) {
        if (player_x == doors[i].x && player_y == doors[i].y) {
            return &(doors[i]);
        }
    }
    return &(empty_door);
}

int get_door_count() {
    return door_count;
}

void clear_doors() {
    door_count = 0;
}