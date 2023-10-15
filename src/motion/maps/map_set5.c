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


void draw_underground_NS() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Underground North-South");

    draw_town_exit(MAP_TOP, MAP_WIDTH - 4);
    add_exit_portal(MAP_TOP, MAP_WIDTH - 4, MAP_CER_CITY);
}
