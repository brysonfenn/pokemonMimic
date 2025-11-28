#include "map_set8.h"

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
#include "../../items/key_items.h"


void draw_cinnabar_island() {
    draw_big_map("Cinnabar Island");
    audio_loop_file("cinnabar.mp3");

    add_connection_to_big_map(MAP_RIGHT, 5, MAP_R16);
    add_connection_to_big_map(MAP_TOP, 48, MAP_R17);

    draw_building_default(MAP_X+6,MAP_Y+8,"POKE",POKE_CENTER_ACTION);
    draw_building_default(MAP_X+15,MAP_Y+8,"MART",MART_ACTION);

    add_surf_selectable(MAP_X+39, MAP_Y+12);
    add_surf_selectable(MAP_X+26, MAP_Y+2);

    draw_building_default(MAP_X+15, MAP_Y+3, "GYM", DOORS_NEW_MAP);
    add_building_portal(MAP_X+15, MAP_Y+3, MAP_GYM7);
    add_locked_door(MAP_X+15+DEFAULT_BUILDING_WIDTH/2, MAP_Y+3+DEFAULT_BUILDING_HEIGHT-1, K_ITEM_SECRET_KEY);
    add_locked_door(MAP_X+15+DEFAULT_BUILDING_WIDTH/2 + 1, MAP_Y+3+DEFAULT_BUILDING_HEIGHT-1, K_ITEM_SECRET_KEY);

    draw_building_default(MAP_X+6,MAP_Y+3,"MNSN",DOORS_NEW_MAP);
    add_portal_building_to_big_map(MAP_X+6, MAP_Y+3, MAP_MANSION1, MAP_BOTTOM, 6);
}

// Mansion layout: 3 4
//                 1 2
void draw_mansion1() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");
    
    add_portal_big_map_to_building(MAP_X+6, MAP_Y+3, MAP_CIN_ISLAND, MAP_BOTTOM, 6);
    add_connection_to_big_map(MAP_RIGHT, 8, MAP_MANSION2);
    add_connection_to_big_map(MAP_RIGHT, 15, MAP_MANSION2);
    add_connection_to_big_map(MAP_TOP, 53, MAP_MANSION3);
    add_connection_to_big_map(MAP_TOP, 41, MAP_MANSION3);
    add_connection_to_big_map(MAP_TOP, 3, MAP_MANSION3);

    add_npc_by_id(MAP_X+1, MAP_Y+10, 29, PLAYER_MOVING_RIGHT);
    add_npc_by_id(MAP_X+34, MAP_Y+13, 29, PLAYER_MOVING_LEFT);
    add_npc_by_id(MAP_X+37, MAP_Y+1, 29, PLAYER_MOVING_LEFT);
    add_npc_by_id(MAP_X+37, MAP_Y+5, 30, PLAYER_MOVING_LEFT);

    //Wall Switch 1
    if (player_record_bit_is_set(RECORD_BIT_18)) {
        draw_wall(MAP_X+1, MAP_Y+7, false, 5, '-');
        draw_wall(MAP_X+30, MAP_Y+11, false, 5, '=');
        draw_wall(MAP_X+23, MAP_Y+3, false, 5, '=');
    }
    //Wall Switch 0
    else {
        draw_wall(MAP_X+8, MAP_Y+8, true, 3, '|');
        draw_wall(MAP_X+1, MAP_Y+7, false, 5, '=');
    }
}

void draw_mansion2() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");

    add_connection_to_big_map(MAP_LEFT, 8, MAP_MANSION1);
    add_connection_to_big_map(MAP_LEFT, 15, MAP_MANSION1);
    add_portal_big_map_to_building(MAP_X+6, MAP_Y+3, MAP_CIN_ISLAND, MAP_BOTTOM, 45);
    add_connection_to_big_map(MAP_TOP, 53, MAP_MANSION4);
    add_connection_to_big_map(MAP_TOP, 42, MAP_MANSION4);

    add_npc_by_id(MAP_X+17, MAP_Y+11, 29, PLAYER_MOVING_LEFT);
    add_npc_by_id(MAP_X+49, MAP_Y+8, 29, PLAYER_MOVING_DOWN);

    //Wall Switch 1
    if (player_record_bit_is_set(RECORD_BIT_18)) {
        draw_wall(MAP_X+1, MAP_Y+13, false, 9, '-');
        draw_wall(MAP_X+51, MAP_Y+12, true, 5, '|');
    }
    //Wall Switch 0
    else {
        draw_wall(MAP_X+1, MAP_Y+13, false, 9, '=');
        draw_wall(MAP_X+23, MAP_Y+14, true, 3, '|');
    }
}

void draw_mansion3() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");

    add_connection_to_big_map(MAP_RIGHT, 2, MAP_MANSION4);
    add_connection_to_big_map(MAP_RIGHT, 9, MAP_MANSION4);
    add_connection_to_big_map(MAP_RIGHT, 15, MAP_MANSION4);
    add_connection_to_big_map(MAP_BOTTOM, 53, MAP_MANSION1);
    add_connection_to_big_map(MAP_BOTTOM, 41, MAP_MANSION1);
    add_connection_to_big_map(MAP_BOTTOM, 3, MAP_MANSION1);

    add_npc_by_id(MAP_X+21, MAP_Y+2, 29, PLAYER_MOVING_RIGHT);
    add_npc_by_id(MAP_X+31, MAP_Y+12, 29, PLAYER_MOVING_DOWN);
    add_npc_by_id(MAP_X+56, MAP_Y+5, 31, PLAYER_MOVING_LEFT);
    add_npc_by_id(MAP_X+25, MAP_Y+12, 32, PLAYER_MOVING_UP);
    add_npc_by_id(MAP_X+1, MAP_Y+3, 33, PLAYER_MOVING_RIGHT);

    //Wall Switch 1
    if (player_record_bit_is_set(RECORD_BIT_18)) {
        draw_wall(MAP_X+39, MAP_Y+3, false, 8, '=');
        draw_wall(MAP_X+23, MAP_Y+11, false, 8, '=');
        draw_wall(MAP_X+10, MAP_Y+1, true, 4, '|');
    }
    //Wall Switch 0
    else {
        draw_wall(MAP_X+34, MAP_Y+8, true, 3, '|');
        draw_wall(MAP_X+22, MAP_Y+14, true, 3, '|');
    }

}

void draw_mansion4() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 53, MAP_MANSION2);
    add_connection_to_big_map(MAP_BOTTOM, 42, MAP_MANSION2);
    add_connection_to_big_map(MAP_LEFT, 2, MAP_MANSION3);
    add_connection_to_big_map(MAP_LEFT, 9, MAP_MANSION3);
    add_connection_to_big_map(MAP_LEFT, 15, MAP_MANSION3);

    add_npc_by_id(MAP_X+56, MAP_Y+4, 29, PLAYER_MOVING_LEFT);
    add_npc_by_id(MAP_X+46, MAP_Y+7, 29, PLAYER_MOVING_LEFT);

    //Wall Switch 1
    if (player_record_bit_is_set(RECORD_BIT_18)) {
        
        draw_wall(MAP_X+38, MAP_Y+13, true, 4, '|');
        draw_wall(MAP_X+27, MAP_Y+9, false, 7, '=');
    }
    //Wall Switch 0
    else {
        draw_wall(MAP_X+47, MAP_Y+1, true, 2, '|');
        draw_wall(MAP_X+27, MAP_Y+9, false, 7, '-');
    }
    
}

void draw_gym7() {
    draw_interior(MAP_X+15, MAP_Y+3, MAP_CIN_ISLAND);
    begin_message_box();
    draw_big_map("Cinnabar Island Gym");
    audio_loop_file("gym.mp3");

    add_trainer_by_id(INTERIOR_X+17, INTERIOR_Y+3, 99, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+21, INTERIOR_Y+3, 100, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+15, INTERIOR_Y+4, 101, PLAYER_MOVING_LEFT);
    add_trainer_by_id(INTERIOR_X+4, INTERIOR_Y+2, 102, PLAYER_MOVING_UP);
    add_trainer_by_id(INTERIOR_X+1, INTERIOR_Y+6, 207, PLAYER_MOVING_UP);
}

void draw_route17() {
    draw_big_map("Route 16");
    audio_loop_file("sea.mp3");

    add_connection_to_big_map(MAP_TOP, 10, MAP_STARTER_TOWN);
    add_connection_to_big_map(MAP_BOTTOM, 48, MAP_CIN_ISLAND);

    add_surf_selectable(MAP_X+26, MAP_Y+4);
    add_surf_selectable(MAP_X+26, MAP_Y+12);
    add_surf_selectable(MAP_X+53, MAP_Y+2);
    add_surf_selectable(MAP_X+55, MAP_Y+13);

    add_trainer_by_id(MAP_X+56, MAP_Y+4, 107, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+30, MAP_Y+6, 108, PLAYER_MOVING_DOWN);
    
}