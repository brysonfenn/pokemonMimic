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

    draw_static_elements();
}

void grass_vir_forest() {
    attrset(COLOR_PAIR(GRASS_COLOR));
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}