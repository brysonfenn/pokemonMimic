#include "wild_pokemon.h"

#include <ncurses.h>

#include "battle.h"
#include "../player.h"
#include "../monsters/pokemon.h"
#include "../print/print_utils.h"
#include "../audio/audio_player.h"

//Battle a random wild pokemon
int battle_wild_pokemon(struct Pokemon * pok) {
    Pokemon example_pokemon = *pok;
    char print_str[256];
    int battle_result = 0;

    audio_loop_file("wild_battle.mp3");

    player.is_trainer_battle = false;

    begin_list();
    
    player_set_current_pokemon(PLAYER_DEFAULT_POKEMON);
    if (player_get_num_alive()) {
        if (example_pokemon.id_num == POKEMON_SNORLAX) sprintf(print_str, "  A wild Snorlax wants to battle!");
        else sprintf(print_str, "  A wild %s appeared!\n", example_pokemon.name);

        print_to_list(print_str);
        sleep(2);
        player.is_uncaught_pokemon = !player_has_pokemon(example_pokemon.id_num);
        battle_result = handle_battle(&example_pokemon);
        player.is_uncaught_pokemon = false;
    }
    else {
        print_to_list("  All Pokemon have fainted, please heal them.\n");
        sleep(2);
        return BATTLE_WHITE_OUT;
    }

    clear();

    //If this pokemon wasn't caught, free all memory allocated for it
    if (battle_result != BATTLE_CAUGHT_POKE) {
        destroy_pokemon(&example_pokemon);
    }

    //Reset stats at the end of a battle
    for (int i = 0; i < player.numInParty; i++) {
      reset_stat_stages(&(player.party[i]));
    }

    player.is_battle = false;

    if (battle_result == BATTLE_WHITE_OUT) {
        return BATTLE_WHITE_OUT;
    }
    else {
        return BATTLE_WIN;
    }
}


int safari_zone_pokemon(struct Pokemon * pok) {
    Pokemon example_pokemon = *pok;
    char print_str[256];
    int battle_result = 0;

    audio_loop_file("wild_battle.mp3");

    player.is_trainer_battle = false;

    begin_list();
    
    player_set_current_pokemon(PLAYER_DEFAULT_POKEMON);
    if (player_get_num_alive()) {
        if (example_pokemon.id_num == POKEMON_SNORLAX) sprintf(print_str, "  A wild Snorlax wants to battle!");
        else sprintf(print_str, "  A wild %s appeared!\n", example_pokemon.name);

        print_to_list(print_str);
        sleep(2);
        player.is_uncaught_pokemon = !player_has_pokemon(example_pokemon.id_num);
        battle_result = handle_battle(&example_pokemon);
        player.is_uncaught_pokemon = false;
    }
    else {
        print_to_list("  All Pokemon have fainted, please heal them.\n");
        sleep(2);
        return BATTLE_WHITE_OUT;
    }

    clear();

    //If this pokemon wasn't caught, free all memory allocated for it
    if (battle_result != BATTLE_CAUGHT_POKE) {
        destroy_pokemon(&example_pokemon);
    }

    //Reset stats at the end of a battle
    for (int i = 0; i < player.numInParty; i++) {
      reset_stat_stages(&(player.party[i]));
    }

    player.is_battle = false;

    return BATTLE_WIN;
}