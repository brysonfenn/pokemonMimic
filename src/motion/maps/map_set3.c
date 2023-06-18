#include "map_set3.h"

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

static Trainer trainer5 = {5, "Bugcatcher Marty", "I'll defeat you with my Bug Pokemon!", 2, {POKEMON_CATERPIE, POKEMON_WEEDLE}, {4,5} };

void draw_vir_forest() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    begin_message_box();
    print_to_message_box("Viridian Forest");

    draw_town_exit(MAP_BOTTOM, 7);
    add_exit_portal(MAP_BOTTOM, 7, MAP_R2);

    draw_town_exit(MAP_TOP, MAP_WIDTH-8);
    add_exit_portal(MAP_TOP, MAP_WIDTH-8, MAP_PEW_CITY);

    add_trainer(MAP_X+11, MAP_Y+7, &trainer5, PLAYER_MOVING_DOWN);

    draw_static_elements();
}

void draw_pew_city() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    begin_message_box();
    print_to_message_box("Pewter City");

    draw_town_exit(MAP_BOTTOM, MAP_WIDTH-8);
    add_exit_portal(MAP_BOTTOM, MAP_WIDTH-8, MAP_VIR_FOREST);

    drawBuilding_default(MAP_X+5,MAP_Y+2, "GYM", -1);
    add_building_portal(MAP_X+5, MAP_Y+2, MAP_GYM1);

    drawBuilding_default(MAP_X+25,MAP_Y+5, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+8,MAP_Y+11, "Mart", MART_ACTION);
}


static Trainer leader201 = {201, "Leader Brock", "I am the First Gym Leader. Let's Battle!", 2, {POKEMON_GEODUDE, POKEMON_ONIX}, {12,14} };
static Trainer trainer6 = {6, "Camper Liam", "I like Rock Type Pokemon. Let's Battle!", 2, {POKEMON_GEODUDE, POKEMON_SANDSHREW}, {10,11} };

void draw_gym1() {
    draw_interior(MAP_X+9, MAP_Y+5, MAP_PEW_CITY);
    begin_message_box();
    print_to_message_box("Pewter City Gym");

    for (int i = 1; i <= 16; i++) {
        mvaddch(INTERIOR_Y+4, INTERIOR_X+i, '-');
    }

    add_trainer(INTERIOR_X+16, INTERIOR_Y+1, &trainer6, PLAYER_MOVING_DOWN);
    add_trainer(INTERIOR_X+1, INTERIOR_Y+1, &leader201, PLAYER_MOVING_RIGHT);
}