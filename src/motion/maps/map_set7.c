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
    draw_big_map("Safari Zones");
    audio_loop_file("evolution_theme.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 25, MAP_FU_CITY);
}

void draw_safari2() {

}

void draw_safari3() {

}

void draw_safari4() {

}