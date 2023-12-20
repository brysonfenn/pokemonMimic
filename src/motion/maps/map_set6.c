#include "map_set6.h"

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


static const int game_corner_rocket_x = INTERIOR_X + INTERIOR_WIDTH - DEFAULT_BUILDING_WIDTH;
static const int game_corner_rocket_y = INTERIOR_Y;


void draw_cel_city() {
    draw_big_map("Celadon City");
    audio_loop_file("celadon_city.mp3");

    add_connection_to_big_map(MAP_RIGHT, 3, MAP_UNDERGROUND);

    drawBuilding_default(MAP_X+35,MAP_Y+1, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+10, MAP_Y+2, "Mart", MART_ACTION);

    drawBuilding_default(MAP_X+18, MAP_Y+7, "GAME", -1);
    add_building_portal(MAP_X+18, MAP_Y+7, MAP_GAME_CORNER);

    drawBuilding_default(MAP_X+1, MAP_Y+12, "Gym", -1);
    add_building_portal(MAP_X+1, MAP_Y+12, MAP_GYM4);

    add_cuttable_tree(MAP_X+31, MAP_Y+15);
    add_cuttable_tree(MAP_X+31, MAP_Y+16);
}


void draw_rocket1() {
    draw_big_map("Rocket Hideout");
    audio_loop_file("rocket_hideout.mp3");

    add_portal_big_map_to_building(game_corner_rocket_x, game_corner_rocket_y, MAP_GAME_CORNER, MAP_BOTTOM, 6);
    add_connection_to_big_map(MAP_TOP, 18, MAP_ROCKET2);
    add_connection_to_big_map(MAP_TOP, 24, MAP_ROCKET2);
    add_connection_to_big_map(MAP_TOP, 44, MAP_ROCKET2);
    add_connection_to_big_map(MAP_RIGHT, 7, MAP_ROCKET3);

    add_trainer_by_id(MAP_X+50, MAP_Y+5, 54, PLAYER_MOVING_UP);
    add_trainer_by_id(MAP_X+18, MAP_Y+7, 55, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+48, MAP_Y+5, 56, PLAYER_MOVING_LEFT);
}

void draw_rocket2() {
    draw_big_map("Rocket Hideout");
    audio_loop_file("rocket_hideout.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 18, MAP_ROCKET1);
    add_connection_to_big_map(MAP_BOTTOM, 24, MAP_ROCKET1);
    add_connection_to_big_map(MAP_BOTTOM, 44, MAP_ROCKET1);

    add_trainer_by_id(MAP_X+11, MAP_Y+7, 57, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+37, MAP_Y+2, 58, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+48, MAP_Y+8, 59, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+31, MAP_Y+12, 61, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+22, MAP_Y+15, 62, PLAYER_MOVING_RIGHT);
}

void draw_rocket3() {
    draw_big_map("Rocket Hideout");
    audio_loop_file("rocket_hideout.mp3");

    add_connection_to_big_map(MAP_LEFT, 7, MAP_ROCKET1);

    if (!((player.record_bits >> 9) & 1)) {
        add_trainer_by_id(MAP_X+34, MAP_Y+14, 60, PLAYER_MOVING_DOWN);
        add_trainer_by_id(MAP_X+48, MAP_Y+4, 63, PLAYER_MOVING_RIGHT);
        add_npc_by_id(MAP_X+49, MAP_Y+1, 13, PLAYER_MOVING_DOWN);
    }
}

void draw_game_corner() {
    draw_interior(MAP_X+18, MAP_Y+7, MAP_CEL_CITY);
    begin_message_box();
    print_to_message_box("Game Corner");
    audio_loop_file("game_corner.mp3");

    draw_box(INTERIOR_X+4, INTERIOR_Y+2, 5, 2);
    draw_box(INTERIOR_X+12, INTERIOR_Y+2, 5, 2);
    add_npc_by_id(INTERIOR_X+3, INTERIOR_Y+2, 11, PLAYER_MOVING_RIGHT);
    add_npc_by_id(INTERIOR_X+7, INTERIOR_Y+4, 12, PLAYER_MOVING_UP);

    drawBuilding_default(game_corner_rocket_x, game_corner_rocket_y, " ", -1);
    add_portal_building_to_big_map(game_corner_rocket_x, game_corner_rocket_y, MAP_ROCKET1, MAP_BOTTOM, 6);

    add_trainer_by_id(game_corner_rocket_x+2, game_corner_rocket_y+DEFAULT_BUILDING_HEIGHT, 53, PLAYER_MOVING_RIGHT);

}

void draw_gym4() {
    draw_interior(MAP_X+1, MAP_Y+12, MAP_CEL_CITY);
    begin_message_box();
    print_to_message_box("Celadon City Gym");
    audio_loop_file("gym.mp3");

    add_trainer_by_id(INTERIOR_X+4, INTERIOR_Y+4, 64, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+21, INTERIOR_Y+4, 65, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+23, INTERIOR_Y+1, 66, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+6, INTERIOR_Y+1, 67, PLAYER_MOVING_DOWN);

    mvaddch(INTERIOR_Y+1, INTERIOR_X+14, ACS_LRCORNER);
    add_trainer_by_id(INTERIOR_X+15, INTERIOR_Y+1, 204, PLAYER_MOVING_DOWN);
    mvaddch(INTERIOR_Y+1, INTERIOR_X+16, ACS_LLCORNER);
}