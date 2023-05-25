#include "generic_map.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../maps.h"

#include "../../print_defines.h"


void draw_generic_map() {

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    begin_message_box();
    print_to_message_box("Unknown Location: look in maps.c to add maps");

    draw_town_exit(MAP_RIGHT, 10);
    add_portal(MAP_X+MAP_WIDTH-1, MAP_Y+10, MAP_X+10, MAP_Y+10, MAP_VIRIDIAN, true);

    grass_generic_map();

    refresh();
}


void grass_generic_map() {
    attrset(COLOR_PAIR(GRASS_COLOR));
    mvaddch(MAP_Y+1, MAP_X+1, GRASS_CHAR); 
    mvaddch(MAP_Y+1, MAP_X+2, GRASS_CHAR); 
    mvaddch(MAP_Y+1, MAP_X+3, GRASS_CHAR); 
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}