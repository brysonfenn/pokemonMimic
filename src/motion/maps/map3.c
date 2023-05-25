#include "map3.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"

#include "../../print_defines.h"


void draw_map3() {

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 25);
    add_exit_portal(MAP_TOP, 25, 1);

    draw_town_exit(MAP_BOTTOM, 41);
    add_exit_portal(MAP_BOTTOM, 41, 4);

    grass_map3();

    mvprintw(MAP_Y+3, MAP_X+1, "-----");
    mvprintw(MAP_Y+4, MAP_X+29, "----------------------------");       
    mvprintw(MAP_Y+9, MAP_X+1, "--------------------------------");
    mvprintw(MAP_Y+13, MAP_X+25, "--------------------------------");

    refresh();
}


void grass_map3() {
    attrset(COLOR_PAIR(GRASS_COLOR));

    int right = MAP_X + MAP_WIDTH - 26;
    int left = MAP_X + 4;

    for (int i = 1; i < 25; i++) {
        mvaddch(MAP_Y+3, left+i+1, GRASS_CHAR);
        mvaddch(MAP_Y+4, left+i, GRASS_CHAR); 
        mvaddch(MAP_Y+5, left+i+2, GRASS_CHAR); 
        mvaddch(MAP_Y+6, left+i+3, GRASS_CHAR); 
        mvaddch(MAP_Y+8, right+i-2, GRASS_CHAR); 
        mvaddch(MAP_Y+9, right+i, GRASS_CHAR); 
        mvaddch(MAP_Y+10, right+i, GRASS_CHAR); 
        mvaddch(MAP_Y+13, MAP_X+i, GRASS_CHAR); 
        mvaddch(MAP_Y+14, MAP_X+i, GRASS_CHAR); 
        mvaddch(MAP_Y+15, MAP_X+i, GRASS_CHAR); 
    }

    for (int i = 0; i < 3; i++) {
        mvaddch(MAP_Y+14+i, right+7, GRASS_CHAR);
        mvaddch(MAP_Y+14+i, right+8, GRASS_CHAR);
        mvaddch(MAP_Y+14+i, right+9, GRASS_CHAR);
    }
    
    
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}