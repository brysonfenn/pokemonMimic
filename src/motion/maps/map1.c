#include "map1.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../maps.h"

#include "../../print/print_utils.h"
#include "../../print/print_defines.h"



#define TRAINER_BATTLE_ACTION 3

void draw_map1() {

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    begin_message_box();
    print_to_message_box("Viridian City");

    drawBuilding_default(MART_X,MART_Y, "Mart", MART_ACTION);
    drawBuilding_default(POKE_CENTER_X,POKE_CENTER_Y, "Poke", POKE_CENTER_ACTION);
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 10); 
    add_exit_portal(MAP_TOP, 10, MAP_R2);
    
    draw_town_exit(MAP_BOTTOM, 25); 
    add_exit_portal(MAP_BOTTOM, 25, MAP_R1);

    refresh();
}