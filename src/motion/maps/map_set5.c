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


void draw_route8() {
    draw_big_map("Route 8");

    add_connection_to_big_map(MAP_LEFT, 8, MAP_CER_CITY);
    add_cuttable_tree(MAP_X+4, MAP_Y+8);
}