#include "doors.h"

#include "location.h"

#define MAX_DOOR_COUNT 30

static Location doors[MAX_DOOR_COUNT];
static int door_count = 0;

void add_door(char x, char y, short action) {
    doors[door_count].x = x;
    doors[door_count].y = y;
    doors[door_count].action = action;
    door_count++;
}

int get_door_action(int player_x, int player_y) {
    for (int i = 0; i < door_count; i++) {
        if (player_x == doors[i].x && player_y == doors[i].y) {
            return doors[i].action;
        }
    }
    return 0;
}

int get_door_count() {
    return door_count;
}

void clear_doors() {
    door_count = 0;
}