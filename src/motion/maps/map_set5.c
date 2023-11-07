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
    print_to_message_box("Underground North-South");

    drawBuilding_default(25, 6, " CC ", -1);
    add_portal(25+9/2, 6+2, MAP_X+MAP_WIDTH-4, MAP_Y+MAP_HEIGHT-2, MAP_CER_CITY, true);

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

    drawBuilding_default(MAP_X+3,MAP_Y+5, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+DEFAULT_BUILDING_WIDTH + 3, MAP_Y+5, "Mart", MART_ACTION);
    drawBuilding_default(MAP_X+30, MAP_Y+12, "Lab", FOSSIL_PROCESS_ACTION);

    if (has_key_item(SS_TICKET) == -1) {
        add_npc_by_id(MAP_X+25, MAP_Y+MAP_HEIGHT-2, 4, PLAYER_MOVING_UP);
        add_npc_by_id(MAP_X+26, MAP_Y+MAP_HEIGHT-2, 4, PLAYER_MOVING_UP);
    }
    else {
        add_npc_by_id(MAP_X+24, MAP_Y+MAP_HEIGHT-2, 5, PLAYER_MOVING_RIGHT);
        add_npc_by_id(MAP_X+27, MAP_Y+MAP_HEIGHT-2, 5, PLAYER_MOVING_LEFT);
    }
    
    add_cuttable_tree(MAP_X+40, MAP_Y+10);
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
}