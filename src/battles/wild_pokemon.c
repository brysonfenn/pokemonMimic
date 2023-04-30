#include "wild_pokemon.h"

#include "../player.h"
#include "../monsters/pokemon.h"

int battle_wild_pokemon() {
    pokemon example_pokemon;
    player.trainer_battle = false;
    
    set_current_pokemon(PLAYER_DEFAULT_POKEMON);
    if (player.numAlive) {
    example_pokemon = *(get_random_wild_pokemon(2, 4));
    clearTerminal();
    printf("A wild %s appeared!\n", example_pokemon.name);
    sleep(3);
    initiate_battle(example_pokemon);
    }
    else {
    printf("All Pokemon have fainted, please heal them.\n");
    sleep(3);
    }
}