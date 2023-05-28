#include "map_vir_forest.h"

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

static Trainer trainer4 = {4, "Trainer Marty", 2, {POKEMON_CATERPIE, POKEMON_WEEDLE}, {4,5} };

void draw_vir_forest() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    begin_message_box();
    print_to_message_box("Viridian Forest");

    draw_town_exit(MAP_BOTTOM, 7);
    add_exit_portal(MAP_BOTTOM, 7, MAP_R2);

    add_trainer(MAP_X+11, MAP_Y+7, &trainer4, PLAYER_MOVING_DOWN);

    attrset(COLOR_PAIR(GRASS_COLOR));
    for (int i = MAP_X+8; i < MAP_X+MAP_WIDTH-1; i++) {
        mvaddch(MAP_Y+3, i, 'Y');
    }
    for (int i = MAP_X+1; i < MAP_X+MAP_WIDTH-10; i++) {
        mvaddch(MAP_Y+6, i, 'Y');
    }
    for (int i = MAP_X+12; i < MAP_X+MAP_WIDTH-1; i++) {
        mvaddch(MAP_Y+10, i, 'Y');
    }
    for (int i = MAP_X+1; i < MAP_X+MAP_WIDTH-10; i++) {
        mvaddch(MAP_Y+14, i, 'Y');
    }
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}

void grass_vir_forest() {
    attrset(COLOR_PAIR(GRASS_COLOR));
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}