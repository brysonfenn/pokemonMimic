#include "map_set3.h"

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


void draw_route3() {
    draw_big_map("Route 3");
    audio_loop_file("route3.mp3");

    //Draw Doors
    add_connection_to_big_map(MAP_LEFT, 8, MAP_PEW_CITY);
    add_connection_to_big_map(MAP_TOP, MAP_WIDTH - 4, MAP_MT_MOON_S);

    draw_building_default(MAP_X+42,MAP_Y+1, "Poke", POKE_CENTER_ACTION);

    add_trainer_by_id(MAP_X+12,MAP_Y+12, 13, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+13,MAP_Y+7, 14, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+56, MAP_Y+8, 37, PLAYER_MOVING_LEFT);
}


void draw_mt_moon_n() {
    draw_big_map("Mt. Moon North");
    audio_loop_file("mt_moon.mp3");

    add_connection_to_big_map(MAP_BOTTOM, MAP_WIDTH - 4, MAP_MT_MOON_S);
    add_connection_to_big_map(MAP_RIGHT, 7, MAP_CER_CITY);

    add_trainer_by_id(MAP_X+26, MAP_Y+11, 15, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+19, MAP_Y+9, 16, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+24, MAP_Y+3, 17, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+51, MAP_Y+5,252, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+41, MAP_Y+1, 19, PLAYER_MOVING_DOWN);

    add_npc_by_id(MAP_X+45, MAP_Y+9, 2, PLAYER_MOVING_LEFT);

    if (!(player.record_bits & 0x01)) {
        draw_building_default(MAP_X+MAP_WIDTH-2-DEFAULT_BUILDING_WIDTH, MAP_Y+1, "FOSL", FOSSIL_ACTION);
    }
}

void draw_mt_moon_s() {
    draw_big_map("Mt. Moon South");
    audio_loop_file("mt_moon.mp3");

    add_connection_to_big_map(MAP_BOTTOM, MAP_WIDTH - 4, MAP_R3);
    add_connection_to_big_map(MAP_TOP, MAP_WIDTH-4, MAP_MT_MOON_N);
}

void draw_cer_city() {
    draw_big_map("Cerulean City");
    audio_loop_file("cer_city.mp3");

    add_connection_to_big_map(MAP_LEFT, 7, MAP_MT_MOON_N);
    add_connection_to_big_map(MAP_TOP, MAP_WIDTH - 5, MAP_R4);
    add_connection_to_big_map(MAP_RIGHT, 8, MAP_R8);

    add_surf_selectable(MAP_X+29, MAP_Y+13);
    add_connection_to_big_map(MAP_BOTTOM, 29, MAP_SAFF_CITY);

    //Bottom-right portal goes to Underground
    draw_town_exit(MAP_BOTTOM, MAP_WIDTH - 4);
    add_portal_big_map_to_building(49, 6, MAP_UNDERGROUND, MAP_BOTTOM, MAP_WIDTH-4);

    draw_building_default(MAP_X+30,MAP_Y+7, "Poke", POKE_CENTER_ACTION);
    draw_building_default(MAP_X+10,MAP_Y+12, "Mart", MART_ACTION);

    draw_building_default(MAP_X+30+DEFAULT_BUILDING_WIDTH,MAP_Y+7, "GYM", DOORS_NEW_MAP);
    add_building_portal(MAP_X+30+DEFAULT_BUILDING_WIDTH, MAP_Y+7, MAP_GYM2);

    if (!player_has_battled_trainer(202)) {
        add_npc_by_id(MAP_X+MAP_WIDTH-4, MAP_Y+MAP_HEIGHT-2, 17, PLAYER_MOVING_UP);
        add_npc_by_id(MAP_X+MAP_WIDTH-3, MAP_Y+MAP_HEIGHT-2, 17, PLAYER_MOVING_UP);
    }
}


void draw_gym2() {
    draw_interior(MAP_X+30+DEFAULT_BUILDING_WIDTH, MAP_Y+7, MAP_CER_CITY);
    begin_message_box();
    print_to_message_box("Cerulean City Gym");
    audio_loop_file("gym.mp3");

    add_trainer_by_id(INTERIOR_X+7, INTERIOR_Y+5, 20, PLAYER_MOVING_LEFT);
    add_trainer_by_id(INTERIOR_X+INTERIOR_WIDTH-2, INTERIOR_Y+4, 21, PLAYER_MOVING_LEFT);

    mvaddch(INTERIOR_Y+1, INTERIOR_X+13, ACS_LRCORNER);
    add_trainer_by_id(INTERIOR_X+14, INTERIOR_Y+1, 202, PLAYER_MOVING_DOWN);
    mvaddch(INTERIOR_Y+1, INTERIOR_X+15, ACS_LLCORNER);
}


void draw_route4() {
    draw_big_map("Route 4");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_BOTTOM, MAP_WIDTH - 5, MAP_CER_CITY);
    add_connection_to_big_map(MAP_RIGHT, 3, MAP_R5);

    add_trainer_by_id(MAP_X+48, MAP_Y+13, 22, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+MAP_WIDTH-2, MAP_Y+11, 23, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+48, MAP_Y+9, 24, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+MAP_WIDTH-2, MAP_Y+7, 25, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+48, MAP_Y+5, 26, PLAYER_MOVING_RIGHT);

    add_trainer_by_id(MAP_X+23, MAP_Y+11, 69, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+1, MAP_Y+5, 70, PLAYER_MOVING_RIGHT);
}


void draw_route5() {
    draw_big_map("Route 5");
    audio_loop_file("bill_route.mp3");

    add_connection_to_big_map(MAP_LEFT, 3, MAP_R4);

    add_npc_by_id(MAP_X+52, MAP_Y+1, 3, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+24, MAP_Y+11, 27, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+48, MAP_Y+7, 28, PLAYER_MOVING_RIGHT);
}