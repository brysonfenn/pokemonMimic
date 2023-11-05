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

    drawBuilding_default(MAP_X+3,MAP_Y+5, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+DEFAULT_BUILDING_WIDTH + 3, MAP_Y+5, "Mart", MART_ACTION);
    drawBuilding_default(MAP_X+30, MAP_Y+12, "Lab", FOSSIL_PROCESS_ACTION);
}