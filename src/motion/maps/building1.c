#include "map4.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"

#include "../../print_defines.h"
#include "../../player.h"
#include "../../print_utils.h"

#define INTERIROR_X 13
#define INTERIOR_Y 2
#define INTERIOR_WIDTH 28
#define INTERIOR_HEIGHT 8

void draw_b1() {

    draw_interior(MAP_X+24, MAP_Y+MAP_HEIGHT-3, 4);

    refresh();
}
