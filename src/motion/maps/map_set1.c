#include "map_set1.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../maps.h"

#include "../../battles/trainer.h"
#include "../../monsters/pokemon_id_list.h"
#include "../../monsters/pokemon.h"
#include "../../print/print_defines.h"
#include "../../player.h"

void draw_starter_town() {
    draw_big_map("Starter Town");
    audio_loop_file("pallet.mp3");

    draw_building_default(MAP_X+6,MAP_Y+7, "Poke", POKE_CENTER_ACTION);
    draw_building_default(MAP_X+20,MAP_Y+12, "LAB", DOORS_NEW_MAP);
    add_building_portal(MAP_X+20, MAP_Y+12, MAP_LAB);
    
    //Draw Doors
    add_connection_to_big_map(MAP_TOP, 41, MAP_R1);

    refresh();
}


void draw_route1() {
    draw_big_map("Route 1");
    audio_loop_file("route1.mp3");
    add_connection_to_big_map(MAP_TOP, 25, MAP_VIRIDIAN);
    add_connection_to_big_map(MAP_BOTTOM, 41, MAP_STARTER_TOWN);
}


void draw_lab() {
    draw_interior(MAP_X+20, MAP_Y+12, MAP_STARTER_TOWN);
    begin_message_box();
    print_to_message_box("Professor's Lab");
    audio_loop_file("pallet.mp3");

    add_trainer_by_id(INTERIOR_X+10, INTERIOR_Y+3, 251, PLAYER_MOVING_DOWN);
}
