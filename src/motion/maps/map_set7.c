#include "map_set7.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../selectables.h"
#include "../maps.h"

#include "../../player.h"
#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"


void draw_safari1() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");

    add_npc_by_id(MAP_X+31, MAP_Y+13, 21, PLAYER_MOVING_DOWN);

    // add_portal_big_map_to_building(MAP_X+18, MAP_Y+1, MAP_FU_CITY, MAP_BOTTOM, 25);
    add_door(MAP_X+25, MAP_Y+MAP_HEIGHT-1, -3, true);
    draw_town_exit(MAP_BOTTOM, 25);

    add_connection_to_big_map(MAP_LEFT, 14, MAP_SAFARI2);
    add_connection_to_big_map(MAP_LEFT, 2, MAP_SAFARI2);
    add_connection_to_big_map(MAP_LEFT, 8, MAP_SAFARI2);

    add_connection_to_big_map(MAP_TOP, 6, MAP_SAFARI4);
    add_connection_to_big_map(MAP_TOP, 53, MAP_SAFARI4);

    add_surf_selectable(MAP_X+4, MAP_Y+5);
    add_surf_selectable(MAP_X+4, MAP_Y+10);
}

void draw_safari2() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");
    
    add_connection_to_big_map(MAP_RIGHT, 14, MAP_SAFARI1);
    add_connection_to_big_map(MAP_RIGHT, 2, MAP_SAFARI1);
    add_connection_to_big_map(MAP_RIGHT, 8, MAP_SAFARI1);

    add_connection_to_big_map(MAP_TOP, 32, MAP_SAFARI3);
    add_connection_to_big_map(MAP_TOP, 6, MAP_SAFARI3);

    add_surf_selectable(MAP_X+5, MAP_Y+5);
    add_surf_selectable(MAP_X+5, MAP_Y+10);
}

void draw_safari3() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 32, MAP_SAFARI2);
    add_connection_to_big_map(MAP_BOTTOM, 6, MAP_SAFARI2);
    add_connection_to_big_map(MAP_RIGHT, 15, MAP_SAFARI4);
    add_connection_to_big_map(MAP_RIGHT, 5, MAP_SAFARI4);
}

void draw_safari4() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 6, MAP_SAFARI1);
    add_connection_to_big_map(MAP_BOTTOM, 53, MAP_SAFARI1);
    add_connection_to_big_map(MAP_LEFT, 15, MAP_SAFARI3);
    add_connection_to_big_map(MAP_LEFT, 5, MAP_SAFARI3);

    add_npc_by_id(MAP_X+42, MAP_Y+10, 22, PLAYER_MOVING_DOWN);
}

void draw_route16() {
    draw_big_map("Route 16");
    audio_loop_file("beach.mp3");

    add_connection_to_big_map(MAP_TOP, 36, MAP_FU_CITY);
}