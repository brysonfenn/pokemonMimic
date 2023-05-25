#include "map4.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"

#include "../../print_defines.h"
#include "../../player.h"

void draw_map4() {

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);

    drawBuilding_default(MAP_X+6,MAP_Y+7, "Poke", POKE_CENTER_ACTION);

    drawBuilding_default(MAP_X+20,MAP_Y+12, "LAB", -1);
    add_building_portal(MAP_X+20, MAP_Y+12, 5);
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 41);
    add_exit_portal(MAP_TOP, 41, 3);

    grass_map4();

    refresh();
}


void grass_map4() {
    attrset(COLOR_PAIR(GRASS_COLOR));
    
    
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}