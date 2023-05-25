#include "map4.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"

#include "../../print_defines.h"
#include "../../player.h"
#include "../../print_utils.h"

#define POKE_CENTER_ACTION 1

#define BUILDING_X 13
#define BUILDING_Y 2
#define BUILDING_WIDTH 28
#define BUILDING_HEIGHT 8

void draw_b1() {

    int min_x = BUILDING_X+10;
    int max_y = BUILDING_Y+BUILDING_HEIGHT-1;

    drawBox(BUILDING_X,BUILDING_Y,BUILDING_WIDTH,BUILDING_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    mvaddch(max_y, min_x-1, ACS_URCORNER);  // Top-right corner
    mvaddch(max_y, min_x, ' ');
    mvaddch(max_y, min_x+1, ' ');
    mvaddch(max_y, min_x+2, ACS_ULCORNER);  // Top-left corner

    add_portal(min_x, max_y, MAP_X+24, MAP_Y+MAP_HEIGHT-3, 4, true);

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