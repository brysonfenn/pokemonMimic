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

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    
    begin_message_box();
    print_to_message_box("Starter Town");

    drawBuilding_default(MAP_X+6,MAP_Y+7, "Poke", POKE_CENTER_ACTION);

    drawBuilding_default(MAP_X+20,MAP_Y+12, "LAB", -1);
    add_building_portal(MAP_X+20, MAP_Y+12, MAP_LAB);
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 41);
    add_exit_portal(MAP_TOP, 41, 3);

    refresh();
}


void draw_route1() {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);

    begin_message_box();
    print_to_message_box("Route 1");
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 25);
    add_exit_portal(MAP_TOP, 25, MAP_VIRIDIAN);

    draw_town_exit(MAP_BOTTOM, 41);
    add_exit_portal(MAP_BOTTOM, 41, MAP_STARTER_TOWN);

    refresh();
}


static Trainer trainer4 = {4, "Rival G", 1, {POKEMON_BULBASAUR}, {5} };

void draw_lab() {
    draw_interior(MAP_X+24, MAP_Y+MAP_HEIGHT-3, MAP_STARTER_TOWN);
    begin_message_box();
    print_to_message_box("Professor's Lab");

    int rival_pok_id = 1;

    //Add Rival
    switch (player.party[0].id_num) {
        case POKEMON_BULBASAUR:
            rival_pok_id = POKEMON_CHARMANDER;
            break;
        case POKEMON_CHARMANDER:
            rival_pok_id = POKEMON_SQUIRTLE;
            break;
        case POKEMON_SQUIRTLE:
            rival_pok_id = POKEMON_BULBASAUR;
            break;
        default:
            rival_pok_id = POKEMON_PIKACHU;
            break;
    }
    trainer4.party_id_list[0] = rival_pok_id;

    add_trainer(player.loc->x, INTERIOR_Y+3, &trainer4, PLAYER_MOVING_DOWN);


    refresh();
}
