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

void draw_mansion1() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");
    
    add_portal_big_map_to_building(MAP_X+6, MAP_Y+3, MAP_CIN_ISLAND, MAP_BOTTOM, 6);

    add_npc_by_id(MAP_X+4, MAP_Y+8, 29, PLAYER_MOVING_DOWN);

    if (player_record_bit_is_set(RECORD_BIT_18)) {
        
    }
    else {
        draw_wall(MAP_X+8, MAP_Y+8, true, 3, '|');
    }
}

void draw_mansion2() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");

}

void draw_mansion3() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");

}

void draw_mansion4() {
    draw_big_map("Pokemon Mansion");
    audio_loop_file("mansion.mp3");
    
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