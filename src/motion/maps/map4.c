#include "map4.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"

#include "../../print_defines.h"
#include "../../player.h"

#define POKE_CENTER_ACTION 1

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

int actions_map4(int player_x, int player_y) {
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
        case POKE_CENTER_ACTION:
            handle_poke_center();
            break;
        default:
            break;
    }

    clear();

    return action;
}

void grass_map4() {
    attrset(COLOR_PAIR(GRASS_COLOR));
    
    
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}