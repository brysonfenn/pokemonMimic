#include "wild_pokemon.h"

#include <ncurses.h>

#include "../player.h"
#include "../monsters/pokemon.h"
#include "../print/print_utils.h"
#include "battle.h"

//Battle a random wild pokemon
int battle_wild_pokemon() {
    Pokemon example_pokemon;
    char print_str[256];
    int battle_result = 0;

    player.trainer_battle = false;

    begin_list();
    
    set_current_pokemon(PLAYER_DEFAULT_POKEMON);
    if (player_get_num_alive()) {
        example_pokemon = *(get_random_wild_pokemon(2, 6));
        sprintf(print_str, "  A wild %s appeared!\n", example_pokemon.name);
        print_to_list(print_str);
        sleep(2);
        battle_result = initiate_battle(&example_pokemon);
    }
    else {
        print_to_list("  All Pokemon have fainted, please heal them.\n");
        sleep(2);
        return BATTLE_WHITE_OUT;
    }

    clear();

    //If this pokemon was caught, free all memory allocated for it
    if (battle_result != BATTLE_CAUGHT_POKE) {
        destroy_pokemon(&example_pokemon);
    }

    //Reset stats at the end of a battle
    for (int i = 0; i < player.numInParty; i++) {
      reset_stat_stages(&(player.party[i]));
    }
    return 0;
}