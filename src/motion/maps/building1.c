#include "map4.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"

#include "../../print_defines.h"
#include "../../player.h"
#include "../../print_utils.h"

#define POKE_CENTER_ACTION 1

#define INTERIROR_X 13
#define INTERIOR_Y 2
#define INTERIOR_WIDTH 28
#define INTERIOR_HEIGHT 8

void draw_b1() {

    draw_interior(MAP_X+24, MAP_Y+MAP_HEIGHT-3, 4);

    refresh();
}

int actions_b1(int player_x, int player_y) {
    Location door = *(get_door(player_x, player_y));
    int action = door.action;
    if (!action) return 0;

    //Handle portal
    if (action == -1) {
        change_map(door.next_map, door.next_x, door.next_y);
        return -1;
    }

    sleep(1);

    clear();

    switch (action) {
        default:
            break;
    }

    clear();

    return action;
}