#include "map_set2.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../maps.h"

#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"

#define VIR_POKE_CENTER_X 28
#define VIR_POKE_CENTER_Y 7
#define VIR_MART_X 44
#define VIR_MART_Y 14


#define TRAINER_BATTLE_ACTION 3

void draw_vir_city() {

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Viridian City");

    drawBuilding_default(VIR_MART_X,VIR_MART_Y, "Mart", MART_ACTION);
    drawBuilding_default(VIR_POKE_CENTER_X, VIR_POKE_CENTER_Y, "Poke", POKE_CENTER_ACTION);

    add_npc_by_id(MAP_X+5, MAP_Y+10, 1, PLAYER_MOVING_RIGHT);
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 10); 
    add_exit_portal(MAP_TOP, 10, MAP_R2);
    
    draw_town_exit(MAP_BOTTOM, 25); 
    add_exit_portal(MAP_BOTTOM, 25, MAP_R1);

    refresh();
}


void draw_route2() {

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Route 2");

    //Draw Doors
    draw_town_exit(MAP_BOTTOM, 10);
    add_exit_portal(MAP_BOTTOM, 10, MAP_VIRIDIAN);

    draw_town_exit(MAP_TOP, 7);
    add_exit_portal(MAP_TOP, 7, MAP_VIR_FOREST);
    draw_town_exit(MAP_TOP, 40);
    add_exit_portal(MAP_TOP, 40, MAP_VIR_FOREST);

    add_trainer_by_id(MAP_X+MAP_WIDTH-2, MAP_Y+MAP_HEIGHT-3, 8, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+1, MAP_Y+8, 1, PLAYER_MOVING_RIGHT);
    add_trainer_by_id(MAP_X+15, MAP_Y+6, 2, PLAYER_MOVING_LEFT);
    add_trainer_by_id(MAP_X+1, MAP_Y+4, 3, PLAYER_MOVING_RIGHT);

    refresh();
}