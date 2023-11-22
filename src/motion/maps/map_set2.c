#include "map_set2.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../maps.h"
#include "../selectables.h"

#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"

#define VIR_POKE_CENTER_X 28
#define VIR_POKE_CENTER_Y 7
#define VIR_MART_X 44
#define VIR_MART_Y 14


#define TRAINER_BATTLE_ACTION 3

void draw_vir_city() {
    draw_big_map("Viridian City");

    drawBuilding_default(VIR_MART_X,VIR_MART_Y, "Mart", MART_ACTION);
    drawBuilding_default(VIR_POKE_CENTER_X, VIR_POKE_CENTER_Y, "Poke", POKE_CENTER_ACTION);
    
    add_connection_to_big_map(MAP_TOP, 10, MAP_R2);
    add_connection_to_big_map(MAP_BOTTOM, 25, MAP_R1);
}


void draw_route2() {
    draw_big_map("Route 2");

    add_connection_to_big_map(MAP_BOTTOM, 10, MAP_VIRIDIAN);
    add_connection_to_big_map(MAP_TOP, 7, MAP_VIR_FOREST);
    add_connection_to_big_map(MAP_TOP, 40, MAP_VIR_FOREST);

    add_trainer_by_id(MAP_X+MAP_WIDTH-2, MAP_Y+MAP_HEIGHT-3, 8, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+1, MAP_Y+8, 1, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+15, MAP_Y+6, 2, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+1, MAP_Y+4, 3, PLAYER_MOVING_RIGHT);
}


void draw_vir_forest() {
    draw_big_map("Viridian Forest");

    add_connection_to_big_map(MAP_BOTTOM, 7, MAP_R2);
    add_connection_to_big_map(MAP_BOTTOM, 40, MAP_R2);
    add_connection_to_big_map(MAP_TOP, MAP_WIDTH-8, MAP_PEW_CITY);
    add_connection_to_big_map(MAP_RIGHT, 2, MAP_R7);

    add_trainer_by_id(MAP_X+11, MAP_Y+7, 5, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+MAP_WIDTH-2, MAP_Y+6, 7, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+1, MAP_Y+3, 36, PLAYER_MOVING_RIGHT);
}


void draw_pew_city() {
    draw_big_map("Pewter City");

    add_connection_to_big_map(MAP_BOTTOM, MAP_WIDTH-8, MAP_VIR_FOREST);
    add_connection_to_big_map(MAP_RIGHT, 8, MAP_R3);
    
    drawBuilding_default(MAP_X+9,MAP_Y+10, "GYM", DOORS_NEW_MAP);
    add_building_portal(MAP_X+9, MAP_Y+10, MAP_GYM1);

    drawBuilding_default(MAP_X+1,MAP_Y+1, "MUS", DOORS_NEW_MAP);
    add_building_portal(MAP_X+1, MAP_Y+1, MAP_MUSEUM);

    add_cuttable_tree(MAP_X+10, MAP_Y+4);

    drawBuilding_default(MAP_X+33,MAP_Y+1, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+26,MAP_Y+6, "Mart", MART_ACTION);

    add_trainer_by_id(MAP_X+45,MAP_Y+1, 9, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+48,MAP_Y+1, 10, PLAYER_MOVING_DOWN);
    add_trainer_by_id(MAP_X+51,MAP_Y+1, 11, PLAYER_MOVING_DOWN);

    if (!has_battled_trainer(201)) {
        for (int i = 0; i < 3; i++) { mvprintw(MAP_Y+7+i,MAP_X+MAP_WIDTH-2, "|"); }
    }
}


void draw_gym1() {
    draw_interior(MAP_X+9, MAP_Y+10, MAP_PEW_CITY);
    begin_message_box();
    draw_big_map("Pewter City Gym");

    for (int i = 1; i <= 16; i++) {
        mvaddch(INTERIOR_Y+4, INTERIOR_X+i, '-');
    }

    add_trainer_by_id(INTERIOR_X+INTERIOR_WIDTH-2, INTERIOR_Y+6, 12, PLAYER_MOVING_LEFT);
    add_trainer_by_id(INTERIOR_X+15, INTERIOR_Y+1, 6, PLAYER_MOVING_DOWN);

    mvaddch(INTERIOR_Y+1, INTERIOR_X+1, ACS_LRCORNER);
    add_trainer_by_id(INTERIOR_X+1, INTERIOR_Y+2, 201, PLAYER_MOVING_RIGHT);
    mvaddch(INTERIOR_Y+3, INTERIOR_X+1, ACS_URCORNER);
}


void draw_museum() {
    draw_interior(MAP_X+1, MAP_Y+1, MAP_PEW_CITY);
    begin_message_box();
    draw_big_map("Pewter City Museum");

    add_npc_by_id(INTERIOR_X+INTERIOR_WIDTH-4, INTERIOR_Y+3, 8, PLAYER_MOVING_RIGHT);
}