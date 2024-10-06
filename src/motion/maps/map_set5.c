#include "map_set5.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../selectables.h"
#include "../maps.h"

#include "../../player.h"
#include "../../items/key_items.h"
#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"


static const int pokemon_tower_gate_x = INTERIOR_X + 6;
static const int pokemon_tower_gate_y = INTERIOR_Y;


void draw_route8() {
    draw_big_map("Route 8");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_LEFT, 8, MAP_CER_CITY);
    add_cuttable_tree(MAP_X+4, MAP_Y+8);

    add_connection_to_big_map(MAP_RIGHT, 6, MAP_R9);

    add_trainer_by_id(MAP_X+17, MAP_Y+7, 41, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+56, MAP_Y+12, 42, PLAYER_MOVING_LEFT);
}

void draw_route9() {
    draw_big_map("Route 9");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_LEFT, 6, MAP_R8);

    draw_building_default(MAP_X+1, MAP_Y+8, "ROCK", -1);
    add_portal_building_to_big_map(MAP_X+1, MAP_Y+8, MAP_ROCK_TUNNEL_N, MAP_BOTTOM, 15);

    add_trainer_by_id(MAP_X+12, MAP_Y+1, 43, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+1, MAP_Y+11, 44, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+9, MAP_Y+12, 45, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+1, MAP_Y+14, 46, PLAYER_MOVING_RIGHT);

    draw_building_default(MAP_X+13, MAP_Y+9, "POKE", POKE_CENTER_ACTION);
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

    draw_building_default(MAP_X+15,MAP_Y+5, "Poke", POKE_CENTER_ACTION);
    draw_building_default(MAP_X+18, MAP_Y+10, "Mart", MART_ACTION);

    draw_building_default(MAP_X+34, MAP_Y+1, "TOWR", -1);
    add_building_portal(MAP_X+34, MAP_Y+1, MAP_TOWER1);

    add_trainer_by_id(MAP_X+4, MAP_Y+9, 47, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+4, MAP_Y+8, 48, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+4, MAP_Y+7, 49, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+4, MAP_Y+6, 50, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+4, MAP_Y+5, 51, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+4, MAP_Y+4, 52, PLAYER_MOVING_LEFT);

}


void draw_tower1() {
    draw_interior(MAP_X+34, MAP_Y+1, MAP_LAV_TOWN);
    begin_message_box();
    print_to_message_box("Pokemon Tower");
    audio_loop_file("poke_tower.mp3");

    draw_building_default(pokemon_tower_gate_x, pokemon_tower_gate_y, " ", -1);
    add_portal_building_to_big_map(pokemon_tower_gate_x, pokemon_tower_gate_y, MAP_TOWER2, MAP_BOTTOM, MAP_WIDTH/2);

    if (has_key_item(K_ITEM_SILPH_SCOPE) == -1) {
        add_npc_by_id(pokemon_tower_gate_x+4, pokemon_tower_gate_y+3, 14, PLAYER_MOVING_DOWN);
        add_npc_by_id(pokemon_tower_gate_x+5, pokemon_tower_gate_y+3, 14, PLAYER_MOVING_DOWN);
    }
    else if ((player.record_bits >> 10) & 1) {
        add_npc_by_id(pokemon_tower_gate_x+3, pokemon_tower_gate_y+3, 15, PLAYER_MOVING_RIGHT);
        add_npc_by_id(pokemon_tower_gate_x+6, pokemon_tower_gate_y+3, 15, PLAYER_MOVING_LEFT);
    }
    else {
        add_npc_by_id(pokemon_tower_gate_x+4, pokemon_tower_gate_y+3, 15, PLAYER_MOVING_DOWN);
        add_npc_by_id(pokemon_tower_gate_x+5, pokemon_tower_gate_y+3, 15, PLAYER_MOVING_DOWN);
    }

    draw_box(INTERIOR_X+1, INTERIOR_Y+2, 2, 2);
    draw_box(INTERIOR_X+1, INTERIOR_Y+5, 2, 2);
    draw_box(INTERIOR_X+4, INTERIOR_Y+2, 2, 2);
    draw_box(INTERIOR_X+4, INTERIOR_Y+5, 2, 2);
    draw_box(INTERIOR_X+18, INTERIOR_Y+2, 2, 2);
    draw_box(INTERIOR_X+18, INTERIOR_Y+5, 2, 2);
    draw_box(INTERIOR_X+22, INTERIOR_Y+2, 2, 2);
    draw_box(INTERIOR_X+22, INTERIOR_Y+5, 2, 2);
}


void draw_tower2() {
    draw_big_map("Pokemon Tower");
    audio_loop_file("poke_tower.mp3");

    add_trainer_by_id(MAP_X+23, MAP_Y+11, 71, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+42, MAP_Y+14, 72, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+15, MAP_Y+7, 73, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+40, MAP_Y+7, 74, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+25, MAP_Y+5, 75, PLAYER_MOVING_RIGHT);

    add_portal_big_map_to_building(pokemon_tower_gate_x, pokemon_tower_gate_y, MAP_TOWER1, MAP_BOTTOM, MAP_WIDTH/2);
    add_connection_to_big_map(MAP_TOP, 27, MAP_TOWER3);
}

void draw_tower3() {
    draw_big_map("Pokemon Tower");
    audio_loop_file("poke_tower.mp3");

    add_trainer_by_id(MAP_X+41, MAP_Y+11, 76, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+49, MAP_Y+7, 77, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+8, MAP_Y+7, 78, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+16, MAP_Y+5, 79, PLAYER_MOVING_LEFT);

    add_connection_to_big_map(MAP_BOTTOM, 27, MAP_TOWER2);
    add_npc_by_id(MAP_X+28, MAP_Y+1, 18, PLAYER_MOVING_DOWN);
}


void draw_route10() {
    draw_big_map("Route 10");
    audio_loop_file("beach.mp3");

    add_connection_to_big_map(MAP_TOP, 38, MAP_LAV_TOWN);
    add_connection_to_big_map(MAP_LEFT, 14, MAP_R6);
    add_connection_to_big_map(MAP_BOTTOM, 36, MAP_R11);

    // add_snorlax(MAP_X+35, MAP_Y+13, 0);
}


void draw_route11() {
    draw_big_map("Route 11");
    audio_loop_file("beach.mp3");

    add_trainer_by_id(MAP_X+51, MAP_Y+7, 80, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+40, MAP_Y+7, 81, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+18, MAP_Y+13, 82, PLAYER_MOVING_DOWN);

    //Add Blocking NPC if player doesn't have 3rd and 4th badge
    if (!(player_has_battled_trainer(203) && player_has_battled_trainer(204))) {
        add_npc_by_id(MAP_X+41, MAP_Y+5, 20, PLAYER_MOVING_LEFT);
    }

    add_connection_to_big_map(MAP_TOP, 36, MAP_R10);
    add_connection_to_big_map(MAP_LEFT, 14, MAP_R12);
}


void draw_route12() {
    draw_big_map("Route 12");
    audio_loop_file("route1.mp3");

    add_cuttable_tree(MAP_X+6, MAP_Y+10); add_cuttable_tree(MAP_X+7, MAP_Y+10);

    add_trainer_by_id(MAP_X+21, MAP_Y+11, 83, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+5, MAP_Y+9, 84, PLAYER_MOVING_RIGHT);
    add_npc_by_id(MAP_X+51, MAP_Y+2, 19, PLAYER_MOVING_LEFT);

    add_connection_to_big_map(MAP_RIGHT, 14, MAP_R11);
    add_connection_to_big_map(MAP_LEFT, 14, MAP_FU_CITY);
}


void draw_fuchsia_city() {
    draw_big_map("Fuchsia City");
    audio_loop_file("fuchsia_city.mp3");

    add_connection_to_big_map(MAP_RIGHT, 14, MAP_R12);

    draw_building_default(MAP_X+4, MAP_Y+11, "GYM", -1);
    add_building_portal(MAP_X+4, MAP_Y+11, MAP_GYM5);
    draw_building_default(MAP_X+13, MAP_Y+11, "POKE", POKE_CENTER_ACTION);
    draw_building_default(MAP_X+22, MAP_Y+11, "MART", MART_ACTION);

    add_connection_to_big_map(MAP_LEFT, 15, MAP_R13);
    
}


void draw_gym5() {
    draw_interior(MAP_X+4, MAP_Y+11, MAP_FU_CITY);
    begin_message_box();
    print_to_message_box("Fuchsia City Gym");
    audio_loop_file("gym.mp3");

    add_trainer_by_id(INTERIOR_X+14, INTERIOR_Y+6, 85, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(INTERIOR_X+20, INTERIOR_Y+4, 86, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+8, INTERIOR_Y+6, 87, PLAYER_MOVING_UP);

    add_trainer_by_id(INTERIOR_X+3, INTERIOR_Y+1, 88, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+23, INTERIOR_Y+1, 89, PLAYER_MOVING_LEFT);
    add_trainer_by_id(INTERIOR_X+1, INTERIOR_Y+2, 90, PLAYER_MOVING_DOWN);

    add_trainer_by_id(INTERIOR_X+25, INTERIOR_Y+3, 205, PLAYER_MOVING_RIGHT);
}


void draw_route13() {
    draw_big_map("Route 13");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_RIGHT, 15, MAP_FU_CITY);
    add_connection_to_big_map(MAP_TOP, 50, MAP_R14);

    add_trainer_by_id(MAP_X+51, MAP_Y+3, 94, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+47, MAP_Y+9, 95, PLAYER_MOVING_RIGHT);
}

void draw_route14() {
    draw_big_map("Route 14");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 50, MAP_R13);
    add_connection_to_big_map(MAP_TOP, 50, MAP_R15);
}

void draw_route15() {
    draw_big_map("Route 15");
    audio_loop_file("route3.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 50, MAP_R14);
    add_connection_to_big_map(MAP_RIGHT, 5, MAP_CEL_CITY);

    add_snorlax(MAP_X+53, MAP_Y+5, 1);

    add_trainer_by_id(MAP_X+42, MAP_Y+7, 91, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+47, MAP_Y+10, 92, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+42, MAP_Y+14, 93, PLAYER_MOVING_DOWN);
}