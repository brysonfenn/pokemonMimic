#include "map4.h"

#include <ncurses.h>

#include "../maps.h"
#include "../map_drawing.h"
#include "../selectables.h"
#include "../location.h"


#include "../../print/print_defines.h"
#include "../../print/print_utils.h"
#include "../../battles/trainer.h"
#include "../../monsters/pokemon_id_list.h"
#include "../../monsters/pokemon.h"
#include "../../player.h"


static Trainer trainer4 = {4, "Rival G", 1, {POKEMON_BULBASAUR}, {5} };

void draw_b1() {
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
