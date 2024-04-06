#include "selectables.h"

#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "npc.h"
#include "map_drawing.h"
#include "motion2d.h"
#include "../player.h"
#include "../battles/trainer.h"
#include "../battles/trainer_list.h"
#include "../battles/battle.h"
#include "../battles/wild_pokemon.h"
#include "../monsters/pokemon.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../audio/audio_player.h"
#include "../items/key_items.h"
#include "../motion/location.h"


//Handle Cutting down a tree
int handle_cut(struct Selectable * selectable_ptr);
int handle_snorlax(struct Selectable * selectable_ptr);


// Handle a selected selectable
int handle_selected_selectable(int player_x, int player_y, char player_char) {
    Selectable * selectable_ptr;
    Trainer * trainer_ptr;
    NPC * npc_ptr;
    char print_str[256];

    selectable_ptr = get_selectable(player_x, player_y, player_char);
    if (selectable_ptr->selectable_id == SELECTABLE_NONE) return SELECTABLE_CONTINUE_WHILE;
    if (selectable_ptr->selectable_id == SELECTABLE_TRAINER) {
        trainer_ptr = (Trainer *) selectable_ptr->data;
        if (player_has_battled_trainer(trainer_ptr->id_num)) {
            sprintf(print_str, "%s: \"We already battled\"", trainer_ptr->name);
            print_to_message_box(print_str); await_user(); begin_message_box();
            return SELECTABLE_CONTINUE_WHILE;
        }

        audio_save_looping_file(0);
        audio_loop_file("trainer_approach.mp3");

        //Handle trainer battle and return values from that function
        if (battle_trainer(trainer_ptr) != BATTLE_WHITE_OUT) { restore_print_state(); }
        audio_restore_looping_file(0);

        return SELECTABLE_CONTINUE_WHILE;
    }
    else if (selectable_ptr->selectable_id == SELECTABLE_NPC) {
        npc_ptr = (NPC *) selectable_ptr->data;
        handle_npc_selection(npc_ptr);
        return SELECTABLE_CONTINUE_WHILE;
    }
    else if (selectable_ptr->selectable_id == SELECTABLE_CUTTABLE_TREE) {
        handle_cut(selectable_ptr);
        return SELECTABLE_CONTINUE_WHILE;
    }
    else if (selectable_ptr->selectable_id == SELECTABLE_SNORLAX) {
        handle_snorlax(selectable_ptr);
        return SELECTABLE_CONTINUE_WHILE;
    }

    return SELECTABLE_BREAK_WHILE;
}


//Handle Cutting down a tree
int handle_cut(struct Selectable * selectable_ptr) {
    char print_str[256];

    print_to_message_box("It looks like this tree can be cut"); await_user();
    Pokemon * curr_pok;
    Pokemon * cut_pok;
    bool has_cut = false;

    for (int i = 0; i < player.numInParty; i++) {
        curr_pok = &(player.party[i]);
        for (int j = 0; j < curr_pok->numAttacks; j++) {
            if (curr_pok->attacks[j].id_num == 206) {
                cut_pok = curr_pok;
                has_cut = true;
            }
        }
    }

    if (has_cut) {
        sprintf(print_str, "Would you like %s to use Cut?\n  Yes\n  No", cut_pok->nickname);
        print_to_message_box(print_str);
        if (get_selection(MAP_HEIGHT+1, 1, 0) == 1) {
            begin_message_box();
            return 0;
        }

        audio_play_file("cut_hm.mp3");
        sprintf(print_str, "%s used Cut!", cut_pok->nickname);
        print_to_message_box(print_str);

        //Flash and Delete Tree, and remove pointer
        for (int i = 0; i < 3; i++) {
            attrset(COLOR_PAIR(TREE_COLOR));
            mvaddch(selectable_ptr->y, selectable_ptr->x, 'T'); refresh();
            usleep(200000);
            attrset(COLOR_PAIR(DEFAULT_COLOR));
            mvaddch(selectable_ptr->y, selectable_ptr->x, ' '); refresh();
            usleep(200000);
        }

        // await_user();


        selectable_ptr->y = 0;
        selectable_ptr->x = 0;
    }

    begin_message_box();

    return 0;
}


int handle_snorlax(struct Selectable * selectable_ptr) {
    char print_str[256];

    print_to_message_box("A huge sleeping Pokemon is blocking the way!"); await_user();
    Pokemon * curr_pok;
    Pokemon * cut_pok;

    if (has_key_item(K_ITEM_SS_TICKET) != -1) {
        print_to_message_box("Would you like to play the Pokemon Flute?\n  Yes\n  No");
        int input = get_selection(MAP_HEIGHT+1, 1, 0);
        if (input == 1 || input == PRESSED_B) {
            begin_message_box();
            return 0;
        }

        // audio_play_file("cut_hm.mp3");
        sprintf(print_str, "%s began playing the Pokemon Flute!", player.name);
        audio_end_loop();
        audio_play_file("poke_flute.mp3");
        print_to_message_box(print_str); sleep(6);

        print_to_message_box("The Pokemon woke up and attacked!"); sleep(2);

        Pokemon * wild_pok = create_new_pokemon(POKEMON_SNORLAX, 30, 0, 0);

        save_print_state();
        audio_play_file("begin_battle.mp3");
        blink_screen(7, restore_print_state);
        begin_message_box(); save_print_state();


        if (battle_wild_pokemon(wild_pok) == BATTLE_WHITE_OUT) {
            change_map(player.loc->map, player.loc->x, player.loc->y);
        }
        else {
            restore_print_state();

            char record_bit_num = * ((char *) selectable_ptr->data);
            player.record_bits |= (1 << record_bit_num);
            Selectable * selectable_ptr2 = get_selectable(player.loc->x, player.loc->y+1, player.player_char);
            if (selectable_ptr2->selectable_id != SELECTABLE_SNORLAX) {
                selectable_ptr2 = get_selectable(player.loc->x, player.loc->y-1, player.player_char);
            }

            mvaddch(selectable_ptr->y, selectable_ptr->x, ' ');
            mvaddch(selectable_ptr2->y, selectable_ptr2->x, ' ');

            selectable_ptr->y = 0;
            selectable_ptr->x = 0;
            selectable_ptr2->y = 0;
            selectable_ptr2->x = 0;
        }
    
        // await_user();
    }

    begin_message_box();

    return 0;
}