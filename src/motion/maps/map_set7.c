#include "map_set7.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../selectables.h"
#include "../maps.h"

#include "../../player.h"
#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"


void draw_safari1() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");

    add_portal_big_map_to_building(MAP_X+18, MAP_Y+1, MAP_FU_CITY, MAP_BOTTOM, 25);
}

void draw_safari2() {

}

void draw_safari3() {

}

void draw_safari4() {

}