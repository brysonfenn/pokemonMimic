#include "map_set4.h"

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../selectables.h"
#include "../maps.h"

#include "../../player.h"
#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"


void draw_route8() {
    draw_big_map("Route 8");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_LEFT, 8, MAP_CER_CITY);
    add_connection_to_big_map(MAP_RIGHT, 6, MAP_R9);
    add_cuttable_tree(MAP_X+4, MAP_Y+8);

    add_trainer_by_id(MAP_X+17, MAP_Y+7, 41, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+56, MAP_Y+12, 42, PLAYER_MOVING_LEFT);
}

void draw_route9() {
    draw_big_map("Route 9");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_LEFT, 6, MAP_R8);

    drawBuilding_default(MAP_X+1, MAP_Y+8, "ROCK", -1);
    add_portal_building_to_big_map(MAP_X+1, MAP_Y+8, MAP_ROCK_TUNNEL_N, MAP_BOTTOM, 15);

    add_trainer_by_id(MAP_X+12, MAP_Y+1, 43, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+1, MAP_Y+11, 44, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+9, MAP_Y+12, 45, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+16, MAP_Y+11, 46, PLAYER_MOVING_DOWN);


}

void draw_rock_tunnel_n() {
    draw_big_map("Rock Tunnel");
    audio_loop_file("mt_moon.mp3");

    draw_town_exit(MAP_BOTTOM, 15);
    add_portal_big_map_to_building(MAP_X+1, MAP_Y+8, MAP_R9, MAP_BOTTOM, 15);

    add_connection_to_big_map(MAP_BOTTOM, 2, MAP_ROCK_TUNNEL_S);
}

void draw_rock_tunnel_s() {
    draw_big_map("Rock Tunnel");
    audio_loop_file("mt_moon.mp3");

    add_connection_to_big_map(MAP_TOP, 2, MAP_ROCK_TUNNEL_N);
    add_connection_to_big_map(MAP_BOTTOM, MAP_WIDTH-4, MAP_LAV_TOWN);
}

void draw_lavender_town() {
    draw_big_map("Lavender Town");
    audio_loop_file("lavender_town.mp3");

    add_connection_to_big_map(MAP_TOP, MAP_WIDTH-4, MAP_ROCK_TUNNEL_S);
    add_connection_to_big_map(MAP_LEFT, 3, MAP_UNDERGROUND);
    add_connection_to_big_map(MAP_BOTTOM, 38, MAP_R10);

    drawBuilding_default(MAP_X+15,MAP_Y+5, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+18, MAP_Y+10, "Mart", MART_ACTION);
}


void draw_cel_city() {
    draw_big_map("Celadon City");
    audio_loop_file("celadon_city.mp3");

    add_connection_to_big_map(MAP_RIGHT, 3, MAP_UNDERGROUND);

    drawBuilding_default(MAP_X+35,MAP_Y+1, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+14, MAP_Y+1, "Mart", MART_ACTION);

    drawBuilding_default(MAP_X+18, MAP_Y+7, "RCKT", -1);
    add_portal_building_to_big_map(MAP_X+18, MAP_Y+7, MAP_ROCKET1, MAP_BOTTOM, 6);
}


void draw_route10() {
    draw_big_map("Route 10");
    audio_loop_file("beach.mp3");

    add_connection_to_big_map(MAP_TOP, 38, MAP_LAV_TOWN);
    add_connection_to_big_map(MAP_LEFT, 14, MAP_R6);

    add_snorlax(MAP_X+13, MAP_Y+13, 0);
}


void draw_rocket1() {
    draw_big_map("Rocket Hideout");
    audio_loop_file("celadon_city.mp3");

    add_portal_big_map_to_building(MAP_X+18, MAP_Y+7, MAP_CEL_CITY, MAP_BOTTOM, 6);
    add_connection_to_big_map(MAP_TOP, 18, MAP_ROCKET2);
    add_connection_to_big_map(MAP_TOP, 24, MAP_ROCKET2);
    add_connection_to_big_map(MAP_TOP, 44, MAP_ROCKET2);
    add_connection_to_big_map(MAP_RIGHT, 7, MAP_ROCKET3);
}

void draw_rocket2() {
    draw_big_map("Rocket Hideout");
    audio_loop_file("celadon_city.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 18, MAP_ROCKET1);
    add_connection_to_big_map(MAP_BOTTOM, 24, MAP_ROCKET1);
    add_connection_to_big_map(MAP_BOTTOM, 44, MAP_ROCKET1);
}

void draw_rocket3() {
    draw_big_map("Rocket Hideout");
    audio_loop_file("celadon_city.mp3");

    add_connection_to_big_map(MAP_LEFT, 7, MAP_ROCKET1);
}