#include "map3.h"

#include "ncurses.h"
#include "map_drawing.h"
#include "location.h"
#include "doors.h"


void draw_map3() {

    drawBox(MAP_X,MAP_Y,TOWN_WIDTH,TOWN_HEIGHT);  //Draw Town border
    mvprintw(22, 1, "Press 'm' to return to the menu");
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 25);
    add_exit_portal(MAP_TOP, 25, 1);

    grass_map3();

    refresh();
}

int actions_map3(int player_x, int player_y) {
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

void grass_map3() {

    attrset(COLOR_PAIR(GRASS_COLOR));
    mvaddch(MAP_Y+10, MAP_X+40, GRASS_CHAR); 
    mvaddch(MAP_Y+10, MAP_X+41, GRASS_CHAR); 
    mvaddch(MAP_Y+10, MAP_X+42, GRASS_CHAR); 
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}