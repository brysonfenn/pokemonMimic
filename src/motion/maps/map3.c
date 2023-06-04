#include "map3.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../maps.h"

#include "../../print/print_defines.h"


void draw_map3() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);

    begin_message_box();
    print_to_message_box("Route 1");
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 25);
    add_exit_portal(MAP_TOP, 25, MAP_VIRIDIAN);

    draw_town_exit(MAP_BOTTOM, 41);
    add_exit_portal(MAP_BOTTOM, 41, MAP_STARTER_TOWN);

    refresh();
}