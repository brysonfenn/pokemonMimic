#include "map4.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../../print_defines.h"
#include "../../print_utils.h"

void draw_b1() {
    draw_interior(MAP_X+24, MAP_Y+MAP_HEIGHT-3, 4);
    begin_message_box();
    print_to_message_box("Professor's Lab");
    refresh();
}
