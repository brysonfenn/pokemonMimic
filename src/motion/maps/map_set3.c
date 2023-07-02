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
static Trainer trainer7 = {7, "Bugcatcher Josh", "I like Bug Pokemon, what about you?", 2, {POKEMON_METAPOD, POKEMON_VENONAT}, {5,6} };

void draw_vir_forest() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Viridian Forest");

    draw_town_exit(MAP_BOTTOM, 7);
    add_exit_portal(MAP_BOTTOM, 7, MAP_R2);
    draw_town_exit(MAP_BOTTOM, 40);
    add_exit_portal(MAP_BOTTOM, 40, MAP_R2);

    draw_town_exit(MAP_TOP, MAP_WIDTH-8);
    add_exit_portal(MAP_TOP, MAP_WIDTH-8, MAP_PEW_CITY);

    add_trainer(MAP_X+11, MAP_Y+7, &trainer5, PLAYER_MOVING_DOWN);
    add_trainer(MAP_X+MAP_WIDTH-2, MAP_Y+6, &trainer7, PLAYER_MOVING_LEFT);

    draw_static_elements();
}

static Trainer trainer9 = {9, "Trainer Allison", "I'm not supposed to be here.", 2, {POKEMON_PIDGEY, POKEMON_MANKEY}, {6,7} };
static Trainer trainer10 = {10, "Trainer Michelle", "I was forced into this.", 2, {POKEMON_MEOWTH, POKEMON_PSYDUCK}, {6,7} };
static Trainer trainer11 = {11, "Trainer Olivia", "I'm not super into this.", 2, {POKEMON_NIDORAN_F, POKEMON_VULPIX}, {7,7} };


void draw_pew_city() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Pewter City");

    draw_town_exit(MAP_BOTTOM, MAP_WIDTH-8);
    add_exit_portal(MAP_BOTTOM, MAP_WIDTH-8, MAP_VIR_FOREST);

    draw_town_exit(MAP_RIGHT, 8);
    add_exit_portal(MAP_RIGHT, 8, MAP_R3);

    drawBuilding_default(MAP_X+5,MAP_Y+2, "GYM", -1);
    add_building_portal(MAP_X+5, MAP_Y+2, MAP_GYM1);

    drawBuilding_default(MAP_X+25,MAP_Y+5, "Poke", POKE_CENTER_ACTION);
    drawBuilding_default(MAP_X+8,MAP_Y+11, "Mart", MART_ACTION);

    add_trainer(MAP_X+45,MAP_Y+1, &trainer9, PLAYER_MOVING_DOWN);
    add_trainer(MAP_X+48,MAP_Y+1, &trainer10, PLAYER_MOVING_DOWN);
    add_trainer(MAP_X+51,MAP_Y+1, &trainer11, PLAYER_MOVING_DOWN);

    if (!has_battled_trainer(201)) {
        for (int i = 0; i < 3; i++) { mvprintw(MAP_Y+7+i,MAP_X+MAP_WIDTH-2, "|"); }
    }
}


static Trainer leader201 = {201, "Leader Brock", "I am the First Gym Leader. Let's Battle!", 2, {POKEMON_GEODUDE, POKEMON_ONIX}, {12,14} };
static Trainer trainer6 = {6, "Camper Liam", "I like Rock Type Pokemon. Let's Battle!", 2, {POKEMON_GEODUDE, POKEMON_SANDSHREW}, {10,11} };
static Trainer trainer12 = {12, "Trainer Jordan", "Stoinks.", 2, {POKEMON_SANDSHREW, POKEMON_DIGLETT}, {7,9} };

void draw_gym1() {
    draw_interior(MAP_X+9, MAP_Y+5, MAP_PEW_CITY);
    begin_message_box();
    print_to_message_box("Pewter City Gym");

    for (int i = 1; i <= 16; i++) {
        mvaddch(INTERIOR_Y+4, INTERIOR_X+i, '-');
    }

    add_trainer(INTERIOR_X+INTERIOR_WIDTH-2, INTERIOR_Y+6, &trainer12, PLAYER_MOVING_LEFT);
    add_trainer(INTERIOR_X+15, INTERIOR_Y+1, &trainer6, PLAYER_MOVING_DOWN);

    mvaddch(INTERIOR_Y+1, INTERIOR_X+1, ACS_LRCORNER);
    add_trainer(INTERIOR_X+1, INTERIOR_Y+2, &leader201, PLAYER_MOVING_RIGHT);
    mvaddch(INTERIOR_Y+3, INTERIOR_X+1, ACS_URCORNER);
}


static Trainer trainer13 = {13, "Trainer Heather", "There's no Rowlett in this Generation.", 2, {POKEMON_ODDISH, POKEMON_PIDGEOTTO}, {8,7} };
static Trainer trainer14 = {14, "Trainer Joey", "I like Starter Pokemon!", 3, {POKEMON_BULBASAUR, POKEMON_CHARMANDER, POKEMON_SQUIRTLE}, {7,7,7} };

void draw_route3() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box("Route 3");

    //Draw Doors
    draw_town_exit(MAP_LEFT, 8);
    add_exit_portal(MAP_LEFT, 8, MAP_PEW_CITY);

    draw_town_exit(MAP_TOP, MAP_WIDTH - 4);
    add_exit_portal(MAP_TOP, MAP_WIDTH - 4, MAP_MT_MOON);

    drawBuilding_default(MAP_X+42,MAP_Y+1, "Poke", POKE_CENTER_ACTION);

    add_trainer(MAP_X+12,MAP_Y+12, &trainer13, PLAYER_MOVING_DOWN);
    add_trainer(MAP_X+13,MAP_Y+7, &trainer14, PLAYER_MOVING_RIGHT);
}