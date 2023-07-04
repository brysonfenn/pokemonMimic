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

static Trainer trainer15 = {15, "Miner Jason", "I have been mining for hours, I need a break.", 2, {POKEMON_GEODUDE, POKEMON_EKANS}, {9,11} };
static Trainer trainer16 = {16, "Trainer Andrew", "This cave is really dark!", 2, {POKEMON_ZUBAT, POKEMON_PARAS}, {10,11} };
static Trainer trainer17 = {17, "Trainer Peter", "I'm looking for cool rocks!", 2, {POKEMON_GEODUDE, POKEMON_MANKEY}, {12,13} };
static Trainer trainer18 = {18, "Trainer Paul", "I caught some evolved Pokemon!", 2, {POKEMON_PARASECT, POKEMON_GRAVELER}, {10,10} };
static Trainer trainer19 = {19, "Trainer Matthew", "I like poison types!", 2, {POKEMON_ZUBAT, POKEMON_EKANS}, {10,10} };

void draw_mt_moon() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Mt. Moon");

    draw_town_exit(MAP_BOTTOM, MAP_WIDTH - 4);
    add_exit_portal(MAP_BOTTOM, MAP_WIDTH - 4, MAP_R3);

    draw_town_exit(MAP_RIGHT, 5);
    add_exit_portal(MAP_RIGHT, 5, MAP_CER_CITY);

    add_trainer(MAP_X+26, MAP_Y+11, &trainer15, PLAYER_MOVING_RIGHT);
    add_trainer(MAP_X+19, MAP_Y+9, &trainer16, PLAYER_MOVING_RIGHT);
    add_trainer(MAP_X+24, MAP_Y+3, &trainer17, PLAYER_MOVING_LEFT);
    add_trainer(MAP_X+47, MAP_Y+3, &trainer18, PLAYER_MOVING_RIGHT);
    add_trainer(MAP_X+41, MAP_Y+1, &trainer19, PLAYER_MOVING_DOWN);
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

static Trainer trainer20 = {20, "Trainer Ariel", "You'll never defeat misty!", 2, {POKEMON_POLIWAG, POKEMON_POLIWHIRL}, {15,17} };
static Trainer trainer21 = {21, "Trainer Katie", "I like water pokemon!", 2, {POKEMON_POLIWAG, POKEMON_PSYDUCK}, {16,17} };
static Trainer leader202 = {202, "Leader Misty", "I am Misty, the Second Gym Leader. Let's Battle!", 2, {POKEMON_STARYU, POKEMON_STARMIE}, {18,21} };

void draw_gym2() {
    draw_interior(MAP_X+30+DEFAULT_BUILDING_WIDTH, MAP_Y+8, MAP_CER_CITY);
    begin_message_box();
    print_to_message_box("Cerulean City Gym");

    add_trainer(INTERIOR_X+7, INTERIOR_Y+5, &trainer20, PLAYER_MOVING_LEFT);
    add_trainer(INTERIOR_X+INTERIOR_WIDTH-2, INTERIOR_Y+4, &trainer21, PLAYER_MOVING_LEFT);
    add_trainer(INTERIOR_X+14, INTERIOR_Y+1, &leader202, PLAYER_MOVING_DOWN);
    
}