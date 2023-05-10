#include "map3.h"

#include "ncurses.h"
#include "map_drawing.h"
#include "location.h"
#include "doors.h"


void draw_map3() {

    drawBox(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    mvprintw(22, 1, "Press 'm' to return to the menu");
    print_btn_instructions(MAP_X+MAP_WIDTH+2, MAP_Y+5);
    
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

    int right = MAP_X + 29;
    int left = MAP_X + 4;

    for (int i = 1; i < 30; i++) {
        mvaddch(MAP_Y+3, left+i+1, GRASS_CHAR); 
        mvaddch(MAP_Y+4, left+i, GRASS_CHAR); 
        mvaddch(MAP_Y+5, left+i+2, GRASS_CHAR); 
        mvaddch(MAP_Y+8, right+i, GRASS_CHAR); 
        mvaddch(MAP_Y+9, right+i, GRASS_CHAR); 
        mvaddch(MAP_Y+10, right+i-1, GRASS_CHAR); 
        mvaddch(MAP_Y+13, MAP_X+i, GRASS_CHAR); 
        mvaddch(MAP_Y+14, MAP_X+i+1, GRASS_CHAR); 
        mvaddch(MAP_Y+15, MAP_X+i+2, GRASS_CHAR); 
    }

    for (int i = 4; i < 15; i++) {
        mvaddch(MAP_Y+i, right, GRASS_CHAR); 
        mvaddch(MAP_Y+i, right+1, GRASS_CHAR); 
        mvaddch(MAP_Y+i, right-2, GRASS_CHAR); 
    }
    
    
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}