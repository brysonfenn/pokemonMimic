#include "map_set4.h"

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../selectables.h"
#include "../maps.h"

#include "../../items/key_items.h"
#include "../../player.h"
#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"


void draw_underground() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Underground");

    drawBuilding_default(49, 6, " CC ", -1);
    add_portal(49+9/2, 6+2, MAP_X+MAP_WIDTH-4, MAP_Y+MAP_HEIGHT-2, MAP_CER_CITY, true);

    draw_town_exit(MAP_BOTTOM, 3);
    add_exit_portal(MAP_BOTTOM, 3, MAP_VERM_CITY);

    draw_town_exit(MAP_RIGHT, 3);
    draw_town_exit(MAP_LEFT, 3);
}


void draw_vermillion_city() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Vermillion City");

    draw_town_exit(MAP_TOP, 3);
    add_exit_portal(MAP_TOP, 3, MAP_UG_NS);

    draw_town_exit(MAP_BOTTOM, 25);
    add_exit_portal(MAP_BOTTOM, 25, MAP_SS_ANNE1);

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

    drawBuilding_default(MAP_X+4, MAP_Y+11, "GYM", -1);
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
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("SS Anne");

    draw_town_exit(MAP_TOP, 25);
    add_exit_portal(MAP_TOP, 25, MAP_VERM_CITY);

    draw_town_exit(MAP_BOTTOM, 3);
    add_exit_portal(MAP_BOTTOM, 3, MAP_SS_ANNE2);
    draw_town_exit(MAP_BOTTOM, 13);
    add_exit_portal(MAP_BOTTOM, 13, MAP_SS_ANNE2);
    draw_town_exit(MAP_BOTTOM, 28);
    add_exit_portal(MAP_BOTTOM, 28, MAP_SS_ANNE2);
    draw_town_exit(MAP_BOTTOM, 42);
    add_exit_portal(MAP_BOTTOM, 42, MAP_SS_ANNE2);
    draw_town_exit(MAP_BOTTOM, 52);
    add_exit_portal(MAP_BOTTOM, 52, MAP_SS_ANNE2);

    add_trainer_by_id(MAP_X+7, MAP_Y+1, 29, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+8, MAP_Y+10, 32, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+25, MAP_Y+11, 33, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+48, MAP_Y+10, 34, PLAYER_MOVING_LEFT);

    add_npc_by_id(MAP_X+53, MAP_Y+1, 6, PLAYER_MOVING_DOWN);
    add_cuttable_tree(MAP_X+49, MAP_Y+1);
}


void draw_ss_anne2() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("SS Anne");

    draw_town_exit(MAP_TOP, 3);
    add_exit_portal(MAP_TOP, 3, MAP_SS_ANNE1);
    draw_town_exit(MAP_TOP, 13);
    add_exit_portal(MAP_TOP, 13, MAP_SS_ANNE1);
    draw_town_exit(MAP_TOP, 28);
    add_exit_portal(MAP_TOP, 28, MAP_SS_ANNE1);
    draw_town_exit(MAP_TOP, 42);
    add_exit_portal(MAP_TOP, 42, MAP_SS_ANNE1);
    draw_town_exit(MAP_TOP, 52);
    add_exit_portal(MAP_TOP, 52, MAP_SS_ANNE1);

    add_trainer_by_id(MAP_X+7, MAP_Y+10, 30, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+36, MAP_Y+11, 31, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+38, MAP_Y+8, 35, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+50, MAP_Y+3, 253, PLAYER_MOVING_RIGHT);
}