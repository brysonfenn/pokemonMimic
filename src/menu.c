#include "menu.h"

#include <stdbool.h>
#include <ncurses.h>

#include "player.h"
#include "print/print_defines.h"
#include "print/print_utils.h"
#include "monsters/pokemon.h"
#include "items.h"

#include "motion/maps.h"

static enum display { TOWN, POKEMON, BAG, PLAYER, SAVE, LOAD,
    POWER_OFF, MAIN } current_display = TOWN;

static bool power_off = false;

void main_menu() {

    int inputNum, inputNum2, return_execute, num_files, selected_poke;
    char example_string[4096];
    int last_selection = 0;
    selected_poke = 0;
    Pokemon tempPok;

    //This is the main menu while loop.
    while(1) {
        for (int i = 0; i < player.numInParty; i++) {
            reset_stat_stages(&(player.party[i]));
        }

        clear();

        //Handle changing the display
        switch (current_display) {

        //This is the actual main menu
        case MAIN:
        begin_list();
        print_to_list("  Back\n  Pokemon\n  Bag\n  Player\n");
        print_to_list("  Save Game\n  Load Game\n  Power Off\n\n");

        inputNum = get_selection(LIST_BOX_Y+1,0,6,last_selection);
        if (inputNum == PRESSED_B) inputNum = TOWN;

        last_selection = inputNum;
        current_display = inputNum;
        clear();
        break;

        //Allow player to walk around. PokeCenter and Mart are here
        case TOWN:
        handle_motion();
        current_display = MAIN;
        break;

        //Handle party changes, releases, and viewing stats
        case POKEMON:
        return_execute = RETURN_TO_PARTY;
        selected_poke = 0;   //Start party selection at position zero

        while (1) {
            if (return_execute == RETURN_TO_PARTY) {
                begin_list();
                printParty();
                print_to_list("  Cancel");
                inputNum = get_selection(LIST_BOX_Y+2,0,player.numInParty,selected_poke);
                if (inputNum == player.numInParty || inputNum == PRESSED_B) { break; }   //Cancel
            }
            return_execute = handle_pokemon_menu(inputNum);
            if (return_execute == RETURN_TO_MENU) break;    //Break if requested
        }

        current_display = MAIN;
        break;

        //Handle items used by player
        case BAG:
        begin_list();
        printBag();
        inputNum = get_selection(LIST_BOX_Y+2,0,player.numInBag,0);
        if (inputNum == player.numInBag || inputNum == PRESSED_B) { current_display = MAIN; break; }
        return_execute = use_item(inputNum, &emptyPok);

        //Return to bag menu if item failed, else back to main menu
        if (return_execute == ITEM_FAILURE || return_execute == ITEM_CATCH_FAILURE) { continue; }
        current_display = MAIN;
        break;
        
        //Display player data
        case PLAYER:
        begin_list();
        printPlayer();
        sleep(2);
        current_display = MAIN;
        break;

        //Save game data to a file
        case SAVE:
        begin_list();
        print_to_list("Select a save file to SAVE to: \n");
        print_save_files();
        inputNum = get_current_save_file();
        inputNum = (inputNum == 0) ? 0 : inputNum-1;  //Adjust to current save file position
        inputNum = get_selection(LIST_BOX_Y+2, 0, 9, inputNum);

        if (inputNum == 9 || inputNum == PRESSED_B) { current_display = MAIN; break; }
        save_game(inputNum+1);
        current_display = MAIN;
        break;

        //Load game data from a file
        case LOAD:
        begin_list();
        print_to_list("Select a save file to LOAD: \n");
        print_save_files();
        inputNum = get_selection(LIST_BOX_Y+2, 0, 9, 0);

        if (inputNum == 9 || inputNum == PRESSED_B) { current_display = MAIN; break; }
        load_game(inputNum+1);
        current_display = MAIN;
        break;

        //Allow loop to break and end game
        case POWER_OFF:
        power_off = true;
        current_display = MAIN;
        break;

        //This should never happen.
        default:
        printw("Ran into default in main loop\n"); refresh(); sleep(1);
        break;
        }

        if (power_off) break;
    }
}