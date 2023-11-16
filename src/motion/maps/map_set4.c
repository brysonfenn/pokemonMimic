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
#include "../../items/key_items.h"

void draw_underground() {
    draw_big_map("Underground");

    drawBuilding_default(49, 6, " CC ", DOORS_NEW_MAP);
    add_portal(49+9/2, 6+2, MAP_X+MAP_WIDTH-4, MAP_Y+MAP_HEIGHT-2, MAP_CER_CITY, true);

    add_connection_to_big_map(MAP_BOTTOM, 3, MAP_VERM_CITY);

    draw_town_exit(MAP_RIGHT, 3);   //Add connection toward Lavendar town
    draw_town_exit(MAP_LEFT, 3);    //Add connection toward Celadon City
}


void draw_vermillion_city() {
    draw_big_map("Vermillion City");

    add_connection_to_big_map(MAP_TOP, 3, MAP_UG_NS);
    add_connection_to_big_map(MAP_BOTTOM, 25, MAP_SS_ANNE1);
    add_connection_to_big_map(MAP_RIGHT, 6, MAP_R6);

    drawBuilding_default(MAP_X+15,MAP_Y+5, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+30, MAP_Y+9, "Lab", FOSSIL_PROCESS_ACTION);
    drawBuilding_default(MAP_X+42, MAP_Y+9, "Mart", MART_ACTION);

    if (has_key_item(SS_TICKET) == -1) {
        add_npc_by_id(MAP_X+25, MAP_Y+MAP_HEIGHT-2, 4, PLAYER_MOVING_UP);
        add_npc_by_id(MAP_X+26, MAP_Y+MAP_HEIGHT-2, 4, PLAYER_MOVING_UP);
    }
    else {
        add_npc_by_id(MAP_X+24, MAP_Y+MAP_HEIGHT-2, 5, PLAYER_MOVING_RIGHT);
        add_npc_by_id(MAP_X+27, MAP_Y+MAP_HEIGHT-2, 5, PLAYER_MOVING_LEFT);
    }

    drawBuilding_default(MAP_X+4, MAP_Y+11, "GYM", DOORS_NEW_MAP);
    add_building_portal(MAP_X+4, MAP_Y+11, MAP_GYM3);
    
    add_cuttable_tree(MAP_X+18, MAP_Y+14);
}


void draw_gym3() {
    draw_interior(MAP_X+4, MAP_Y+11, MAP_VERM_CITY);
    begin_message_box();
    print_to_message_box("Vermillion City Gym");

    add_trainer_by_id(INTERIOR_X+14, INTERIOR_Y+1, 203, PLAYER_MOVING_DOWN);
}


void draw_ss_anne1() {
    draw_big_map("SS Anne (Upper)");

    add_connection_to_big_map(MAP_TOP, 25, MAP_VERM_CITY);
    add_connection_to_big_map(MAP_BOTTOM, 3, MAP_SS_ANNE2);
    add_connection_to_big_map(MAP_BOTTOM, 13, MAP_SS_ANNE2);
    add_connection_to_big_map(MAP_BOTTOM, 28, MAP_SS_ANNE2);
    add_connection_to_big_map(MAP_BOTTOM, 42, MAP_SS_ANNE2);
    add_connection_to_big_map(MAP_BOTTOM, 52, MAP_SS_ANNE2);

    add_trainer_by_id(MAP_X+7, MAP_Y+1, 29, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+8, MAP_Y+10, 32, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+25, MAP_Y+11, 33, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+48, MAP_Y+10, 34, PLAYER_MOVING_LEFT);

    add_npc_by_id(MAP_X+53, MAP_Y+1, 6, PLAYER_MOVING_DOWN);
    add_cuttable_tree(MAP_X+49, MAP_Y+1);
}


void draw_ss_anne2() {
    draw_big_map("SS Anne (Lower)");

    add_connection_to_big_map(MAP_TOP, 3, MAP_SS_ANNE1);
    add_connection_to_big_map(MAP_TOP, 13, MAP_SS_ANNE1);
    add_connection_to_big_map(MAP_TOP, 28, MAP_SS_ANNE1);
    add_connection_to_big_map(MAP_TOP, 42, MAP_SS_ANNE1);
    add_connection_to_big_map(MAP_TOP, 52, MAP_SS_ANNE1);

    add_trainer_by_id(MAP_X+7, MAP_Y+10, 30, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+36, MAP_Y+11, 31, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+38, MAP_Y+8, 35, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+50, MAP_Y+9, 253, PLAYER_MOVING_RIGHT);
}


void draw_route6() {
    draw_big_map("Route 6");
    add_connection_to_big_map(MAP_LEFT, 6, MAP_VERM_CITY);

    //Draw Building and link to SouthEast Diglett's Cave
    drawBuilding_default(MAP_X+2, MAP_Y+1, "CAVE", DOORS_NEW_MAP);
    add_portal(MAP_X+2+9/2, MAP_Y+1+2, MAP_X+MAP_WIDTH-4, MAP_Y+MAP_HEIGHT-2, MAP_DIG_CAVE, true);

    add_npc_by_id(MAP_X+1, MAP_Y+12, 7, PLAYER_MOVING_RIGHT);
    add_npc_by_id(MAP_X+5, MAP_Y+4, 9, PLAYER_MOVING_DOWN);
}


void draw_dig_cave() {
    draw_big_map("Diglett's Cave");
    
    //Bottom-right portal goes to route 6
    draw_town_exit(MAP_BOTTOM, MAP_WIDTH - 4);
    add_portal(MAP_X+MAP_WIDTH-4, MAP_Y+MAP_HEIGHT-1, MAP_X+2+9/2, MAP_Y+1+2+1, MAP_R6, true);

    //Bottom-left portal goes to route 7
    draw_town_exit(MAP_BOTTOM, 2);
    add_portal(MAP_X+2, MAP_Y+MAP_HEIGHT-1, MAP_X+47+9/2, MAP_Y+12+2+1, MAP_R7, true);
}


void draw_route7() {
    draw_big_map("Route 7");

    //Draw Building and link to South-West Diglett's Cave
    drawBuilding_default(MAP_X+47, MAP_Y+12, "CAVE", DOORS_NEW_MAP);
    add_portal(MAP_X+47+9/2, MAP_Y+12+2, MAP_X+2, MAP_Y+MAP_HEIGHT-2, MAP_DIG_CAVE, true);

    add_connection_to_big_map(MAP_LEFT, 2, MAP_VIR_FOREST);
    add_cuttable_tree(MAP_X+10, MAP_Y+2);
}