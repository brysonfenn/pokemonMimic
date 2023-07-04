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


void draw_mt_moon() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Mt. Moon");

    draw_town_exit(MAP_BOTTOM, MAP_WIDTH - 4);
    add_exit_portal(MAP_BOTTOM, MAP_WIDTH - 4, MAP_R3);

    draw_town_exit(MAP_RIGHT, 5);
    add_exit_portal(MAP_RIGHT, 5, MAP_CER_CITY);
}

void draw_cer_city() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Cerulean City");

    draw_town_exit(MAP_LEFT, 5);
    add_exit_portal(MAP_LEFT, 5, MAP_MT_MOON);

    drawBuilding_default(MAP_X+30,MAP_Y+8, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+10,MAP_Y+12, "Mart", MART_ACTION);

    drawBuilding_default(MAP_X+30+DEFAULT_BUILDING_WIDTH,MAP_Y+8, "GYM", -1);
    add_building_portal(MAP_X+30+DEFAULT_BUILDING_WIDTH, MAP_Y+8, MAP_GYM2);
}


static Trainer leader202 = {202, "Leader Misty", "I am Misty, the Second Gym Leader. Let's Battle!", 2, {POKEMON_SQUIRTLE, POKEMON_WARTORTLE}, {21,22} };

void draw_gym2() {
    draw_interior(MAP_X+30+DEFAULT_BUILDING_WIDTH, MAP_Y+8, MAP_CER_CITY);
    begin_message_box();
    print_to_message_box("Cerulean City Gym");

    add_trainer(INTERIOR_X+12, INTERIOR_Y+1, &leader202, PLAYER_MOVING_DOWN);
}