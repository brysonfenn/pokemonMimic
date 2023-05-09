#include "menu.h"

#include <stdbool.h>

#include "player.h"
#include "print_defines.h"
#include "monsters/pokemon.h"
#include "items.h"

static enum display { WILD, TRAINER, POKEMON, BAG, PLAYER, SAVE, LOAD,
    TOWN, POWER_OFF, MAIN } current_display = TOWN;

static bool power_off = false;

void main_menu() {

    int inputNum, inputNum2, return_execute, num_files;
    char example_string[100];
    int last_selection = 0;
    pokemon tempPok;

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
        printw("  Wild Pokemon\n  Trainer\n  Pokemon\n  Bag\n  Player\n");
        printw("  Save Game\n  Load Game\n  Town\n  Power Off\n\n");

        inputNum = get_selection(0,0,8,last_selection, MAIN_SELECT);
        last_selection = inputNum;
        current_display = inputNum;
        clear();
        break;

        //Battle wild pokemon
        case WILD:
        battle_wild_pokemon();
        current_display = MAIN;
        break;

        //Battle a trainer
        case TRAINER:
        battle_trainer();
        current_display = MAIN;
        break;

        //Handle party changes, releases, and viewing stats
        case POKEMON:
        clear();
        printParty();
        printw("  Cancel\n\n", 0);
        inputNum = get_selection(1,0,player.numInParty,0, NOT_MAIN_SELECT);
        if (inputNum == player.numInParty) { current_display = MAIN; break; }
        

        clear();
        print_pokemon_summary(&(player.party[inputNum]));
        printw("\n  Switch\n  Release\n  Cancel\n");
        inputNum2 = get_selection(POKE_SUMMARY_SEL_BEGIN, 0, 2, 0, NOT_MAIN_SELECT);

        //Break if inputNum2 is 2 (cancel)
        if (inputNum2 == 2) { break; } 
        //Switch
        else if (inputNum2 == 0) {
            if (player.numInParty <= 1) {printw("You only have 1 Pokémon!\n"); refresh(); sleep(2); break; }
            clear();
            printw("Which pokemon would you like to switch with %s?\n", player.party[inputNum].name);
            printParty();
            printw("\n");
            inputNum2 = get_selection(2,0,player.numInParty-1,0, NOT_MAIN_SELECT);

            tempPok = player.party[inputNum];
            player.party[inputNum] = player.party[inputNum2];
            player.party[inputNum2] = tempPok;
        }
        //Release
        else if (inputNum2 == 1) {
            if (player.numInParty <= 1) {printw("You only have 1 Pokémon!\n"); refresh(); sleep(2); break; }
            clear();
            printw("Are you sure you want to release %s?\n  Yes\n  No\n", player.party[inputNum].name);
            inputNum2 = get_selection(1,0,1,0, NOT_MAIN_SELECT);
            if (inputNum2) { break; }
            else {
            clear();
            printw("Bye Bye, %s!\n", player.party[inputNum].name); refresh(); sleep(2);
            player.numInParty--;
            for (int i = inputNum; i < player.numInParty; i++) {
                player.party[i] = player.party[i+1];
            }
            player.party[player.numInParty] = emptyPok;
            }
        }
        current_display = MAIN;
        break;

        //Handle items used by player
        case BAG:
        clear();
        printBag();
        inputNum = get_selection(1,0,player.numInBag,0, NOT_MAIN_SELECT);
        if (inputNum == player.numInBag) { current_display = MAIN; break; }
        return_execute = use_item(inputNum, &emptyPok);

        //Return to bag menu if item failed, else back to main menu
        if (return_execute == ITEM_FAILURE || return_execute == ITEM_CATCH_FAILURE) { continue; }
        current_display = MAIN;
        break;
        
        //Display player data
        case PLAYER:
        printPlayer();
        current_display = MAIN;
        break;

        //Save game data to a file
        case SAVE:
        clear();
        printw("Select a save file to save to: \n");
        print_save_files();
        inputNum = get_current_save_file();
        inputNum = (inputNum == 0) ? 0 : inputNum-1;  //Adjust to current save file position
        inputNum = get_selection(1, 0, 9, inputNum, NOT_MAIN_SELECT);
        clear();
        if (inputNum == 9) { current_display = MAIN; break; }
        save_game(inputNum+1);
        current_display = MAIN;
        break;

        //Load game data from a file
        case LOAD:
        clear();
        printw("Select a save file to load: \n");
        print_save_files();
        inputNum = get_selection(1, 0, 9, 0, NOT_MAIN_SELECT);
        clear();
        if (inputNum == 9) { current_display = MAIN; break; }
        load_game(inputNum+1);
        current_display = MAIN;
        break;
        
        //Allow player to walk around. PokeCenter and Mart are here
        case TOWN:
        handle_motion();
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