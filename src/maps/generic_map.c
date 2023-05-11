#include "generic_map.h"

#include "ncurses.h"
#include "map_drawing.h"
#include "location.h"
#include "doors.h"
#include "../print_defines.h"


void draw_generic_map() {

    drawBox(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y);

    mvprintw(MAP_Y+5, MAP_X+5, "Generic Map: look in maps.c to add maps");

    draw_town_exit(MAP_RIGHT, 10);
    add_portal(MAP_X+MAP_WIDTH-1, MAP_Y+10, MAP_X+10, MAP_Y+10, 1, true);

    grass_generic_map();

    refresh();
}

int actions_generic_map(int player_x, int player_y) {
    Location door = *(get_door(player_x, player_y));
    int action = door.action;
    if (!action) return 0;

    //Handle portal
    if (action == -1) {
        change_map(door.next_map, door.next_x, door.next_y);
        return -1;
    }

    return 0;
}

void grass_generic_map() {
    attrset(COLOR_PAIR(GRASS_COLOR));
    mvaddch(MAP_Y+1, MAP_X+1, GRASS_CHAR); 
    mvaddch(MAP_Y+1, MAP_X+2, GRASS_CHAR); 
    mvaddch(MAP_Y+1, MAP_X+3, GRASS_CHAR); 
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}