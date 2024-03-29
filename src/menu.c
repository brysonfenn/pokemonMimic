#include "menu.h"

#include <stdbool.h>
#include <ncurses.h>

#include "player.h"
#include "print/print_defines.h"
#include "print/print_utils.h"
#include "monsters/pokemon.h"
#include "items/items.h"
#include "items/key_items.h"
#include "items/hm_tms.h"
#include "motion/maps.h"

static enum display { EXIT_MENU, POKEMON, BAG, KEY_ITEMS, HM_or_TMs, PLAYER, SAVE, LOAD,
    POWER_OFF, MENU } current_display = MENU;

static bool power_off = false;


//Handle start menu
int main_menu() {

    int inputNum, inputNum2, return_execute, num_files, selected_item;
    char print_str[1024] = "";
    int last_selection = 0;
    selected_item = 0;
    Pokemon tempPok;

    //This is the main menu while loop.
    while(1) {

        clear();

        //Handle changing the display
        switch (current_display) {

        //This is the actual main menu
        case MENU:
            begin_list();
            sprintf(print_str, "  Back\n  Pokemon\n  Bag\n  Key Items\n  HMs/TMs\n  %s\n", player.name);
            sprintf(print_str, "%s  Save Game\n  Load Game\n  Power Off\n\n", print_str);
            print_to_list(print_str);

            inputNum = get_selection(0, 8, last_selection);
            if (inputNum == PRESSED_B) inputNum = EXIT_MENU;

            last_selection = inputNum;
            current_display = inputNum;
            clear();
            break;

        //Allow player to walk around. PokeCenter and Mart are here
        case EXIT_MENU:
            current_display = MENU;
            return MENU_RETURN_DEFAULT;

        //Handle party changes, releases, and viewing stats
        case POKEMON:
            return_execute = RETURN_TO_PARTY;
            selected_item = 0;   //Start party selection at position zero

            while (1) {
                if (return_execute == RETURN_TO_PARTY) {
                    begin_list();
                    printParty();
                    print_to_list("  Cancel");
                    selected_item = get_selection(1, player.numInParty, selected_item);
                    if (selected_item == player.numInParty || selected_item == PRESSED_B) { break; }   //Cancel
                }
                return_execute = handle_pokemon_menu(selected_item);
                if (return_execute == RETURN_TO_MENU) break;    //Break if requested
            }

            current_display = MENU;
            break;

        //Handle items used by player
        case BAG:
            return_execute = ITEM_FAILURE;
            selected_item = 0;

            //Return to bag menu if item failed, else back to MENU menu
            while (return_execute == ITEM_FAILURE || return_execute == ITEM_CATCH_FAILURE) {
                begin_list();
                printBag();
                selected_item = get_selection(1, player.numInBag, selected_item);
                if (selected_item == player.numInBag || selected_item == PRESSED_B) { break; }
                return_execute = use_item(selected_item);
            }

            current_display = MENU;
            break;

        case KEY_ITEMS:
            begin_list();
            print_to_list("Key Items:");
            for (int i = 0; i < player.numKeyItems; i++) {
                sprintf(print_str, "  %s\n", get_key_item_name(player.key_items[i]));
                print_to_list(print_str);
            }
            print_to_list(" \n  Cancel");
            get_selection(player.numKeyItems+2,0,0);
            current_display = MENU;
            break;

        case HM_or_TMs:
            begin_list();
            print_to_list("HM/TMs:");
            for (int i = 0; i < player.numHMTMs; i++) {
                sprintf(print_str, "  %s\n", get_hm_tm_name(player.hm_tms[i]));
                print_to_list(print_str);
            }
            print_to_list("  Cancel");
            return_execute = get_selection(1, player.numHMTMs, 0);
            
            if (return_execute == player.numHMTMs || return_execute == PRESSED_B) {
                
            }
            else {
                teach_hm_tm(player.hm_tms[return_execute]);
            }
            
            current_display = MENU;
            break;
        
        //Display player data
        case PLAYER:
            begin_list();
            player_print();
            print_to_list("\n \n  Change Player Name\n  Cancel\n");
            inputNum = get_selection(8,1,1);

            //Change name if input is 0
            if (inputNum == 0) {
                sprintf(print_str, "%s", player.name);
                sprintf(player.name, "%s", get_name_input("yourself"));
                if (strcmp(player.name, DEFAULT_NAME_STR) == 0) sprintf(player.name, "%s", print_str);
            }

            current_display = MENU;
            break;

        //Save game data to a file
        case SAVE:
            begin_list();
            print_to_list("Select a save file to SAVE to: \n");
            print_save_files();
            inputNum = get_current_save_file();
            inputNum = (inputNum == 0) ? 0 : inputNum-1;  //Adjust to current save file position
            inputNum = get_selection(1, 9, inputNum);

            if (inputNum == 9 || inputNum == PRESSED_B) { current_display = MENU; break; }
            save_game(inputNum+1);
            current_display = MENU;
            break;

        //Load game data from a file
        case LOAD:
            begin_list();
            print_to_list("Select a save file to LOAD: \n");
            print_save_files();
            inputNum = get_selection(1, 9, 0);

            if (inputNum == 9 || inputNum == PRESSED_B) { current_display = MENU; break; }
            load_game(inputNum+1);
            current_display = MENU;

            return MENU_LOADED_GAME;

        //Allow loop to break and end game
        case POWER_OFF:
            power_off = true;
            current_display = MENU;
            break;

        //This should never happen.
        default:
            printw("Ran into default in menu loop\n"); refresh(); sleep(1);
            break;
        }

        if (power_off) return MENU_POWER_OFF;

    }

    return MENU_RETURN_DEFAULT;
}