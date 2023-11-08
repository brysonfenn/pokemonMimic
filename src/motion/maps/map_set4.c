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


void draw_mt_moon() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Mt. Moon");

    draw_town_exit(MAP_BOTTOM, MAP_WIDTH - 4);
    add_exit_portal(MAP_BOTTOM, MAP_WIDTH - 4, MAP_R3);

    draw_town_exit(MAP_RIGHT, 7);
    add_exit_portal(MAP_RIGHT, 7, MAP_CER_CITY);

    add_trainer_by_id(MAP_X+26, MAP_Y+11, 15, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+19, MAP_Y+9, 16, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+24, MAP_Y+3, 17, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+51, MAP_Y+5,252, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+41, MAP_Y+1, 19, PLAYER_MOVING_DOWN);

    add_npc_by_id(MAP_X+45, MAP_Y+9, 2, PLAYER_MOVING_LEFT);

    drawBuilding_default(MAP_X+MAP_WIDTH-2-DEFAULT_BUILDING_WIDTH, MAP_Y+1, "FOSL", FOSSIL_ACTION);
}

void draw_cer_city() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Cerulean City");

    draw_town_exit(MAP_LEFT, 7);
    add_exit_portal(MAP_LEFT, 7, MAP_MT_MOON);

    draw_town_exit(MAP_TOP, MAP_WIDTH - 5);
    add_exit_portal(MAP_TOP, MAP_WIDTH - 5, MAP_R4);

    //Bottom-right portal goes to Underground
    draw_town_exit(MAP_BOTTOM, MAP_WIDTH - 4);
    add_portal(MAP_X+MAP_WIDTH-4, MAP_Y+MAP_HEIGHT-1, 49+9/2, 6+2+1, MAP_UG_NS, true);

    drawBuilding_default(MAP_X+30,MAP_Y+8, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+10,MAP_Y+12, "Mart", MART_ACTION);

    drawBuilding_default(MAP_X+30+DEFAULT_BUILDING_WIDTH,MAP_Y+8, "GYM", -1);
    add_building_portal(MAP_X+30+DEFAULT_BUILDING_WIDTH, MAP_Y+8, MAP_GYM2);

    if (!has_battled_trainer(202)) {
        for (int i = 0; i < 4; i++) { mvprintw(MAP_Y+MAP_HEIGHT-2,MAP_X+MAP_WIDTH-5+i, "_"); }
    }
}


void draw_gym2() {
    draw_interior(MAP_X+30+DEFAULT_BUILDING_WIDTH, MAP_Y+8, MAP_CER_CITY);
    begin_message_box();
    print_to_message_box("Cerulean City Gym");

    add_trainer_by_id(INTERIOR_X+7, INTERIOR_Y+5, 20, PLAYER_MOVING_LEFT);
    add_trainer_by_id(INTERIOR_X+INTERIOR_WIDTH-2, INTERIOR_Y+4, 21, PLAYER_MOVING_LEFT);
    add_trainer_by_id(INTERIOR_X+14, INTERIOR_Y+1, 202, PLAYER_MOVING_DOWN);
}


void draw_route4() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Route 4");

    //Draw Doors
    draw_town_exit(MAP_BOTTOM, MAP_WIDTH - 5);
    add_exit_portal(MAP_BOTTOM, MAP_WIDTH - 5, MAP_CER_CITY);

    draw_town_exit(MAP_RIGHT, 3);
    add_exit_portal(MAP_RIGHT, 3, MAP_R5);

    add_trainer_by_id(MAP_X+48, MAP_Y+13, 22, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+MAP_WIDTH-2, MAP_Y+11, 23, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+48, MAP_Y+9, 24, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+MAP_WIDTH-2, MAP_Y+7, 25, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+48, MAP_Y+5, 26, PLAYER_MOVING_RIGHT);
}


void draw_route5() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Route 5");

    draw_town_exit(MAP_LEFT, 3);
    add_exit_portal(MAP_LEFT, 3, MAP_R4);

    add_npc_by_id(MAP_X+52, MAP_Y+1, 3, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+24, MAP_Y+11, 27, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+48, MAP_Y+7, 28, PLAYER_MOVING_RIGHT);
}