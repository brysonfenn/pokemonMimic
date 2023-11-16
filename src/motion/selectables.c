#include "selectables.h"

#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../battles/trainer.h"
#include "../battles/trainer_list.h"
#include "../battles/battle.h"
#include "../monsters/pokemon.h"

#include "map_drawing.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../player.h"
#include "npc.h"

#define HORIZONTAL_DISTANCE 10
#define VERTICAL_DISTANCE 4

static Selectable selectables[30];
static int num_selectables = 0;

const char * empty_string = "NONE";
static Selectable empty_selectable = {0, 0, SELECTABLE_NONE};


//Add a trainer by ID Number at a given location (See trainer_list.c)
void add_trainer_by_id(char x, char y, int trainer_id, char face_direction) {
    //initialize empty selectable
    empty_selectable.data = empty_string;

    struct Trainer * trainer = get_trainer(trainer_id);

    Selectable new_selectable;
    new_selectable.data = trainer;
    new_selectable.x = x;
    new_selectable.y = y;
    new_selectable.selectable_id = SELECTABLE_TRAINER;

    selectables[num_selectables] = new_selectable;
    num_selectables++;

    attrset(COLOR_PAIR(TRAINER_COLOR));
    mvaddch(y,x,face_direction); refresh();
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}


//Add npc at a given location
void add_npc_by_id(char x, char y, int npc_id, char face_direction) {
    //initialize empty selectable
    empty_selectable.data = empty_string;

    struct NPC * npc = get_npc(npc_id);

    Selectable new_selectable;
    new_selectable.data = npc;
    new_selectable.x = x;
    new_selectable.y = y;
    new_selectable.selectable_id = SELECTABLE_NPC;

    selectables[num_selectables] = new_selectable;
    num_selectables++;

    attrset(COLOR_PAIR(NPC_COLOR));
    mvaddch(y,x,face_direction); refresh();
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}


//Add Cuttable Tree at a location
void add_cuttable_tree(char x, char y) {
    Selectable new_selectable;
    new_selectable.data = empty_string;
    new_selectable.x = x;
    new_selectable.y = y;
    new_selectable.selectable_id = SELECTABLE_CUTTABLE_TREE;
    
    selectables[num_selectables] = new_selectable;
    num_selectables++;

    attrset(COLOR_PAIR(TREE_COLOR));
    mvaddch(y,x,'T'); refresh();
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}


// Return a door (if there is one) at player location, else return zero-door
Selectable * get_selectable(int player_x, int player_y, char player_char) {
    char trainer_char, trainer_color;
    bool is_person = false;

    if (player_char == PLAYER_MOVING_LEFT) { player_x--; trainer_char = PLAYER_MOVING_RIGHT; }
    else if (player_char == PLAYER_MOVING_RIGHT) { player_x++; trainer_char = PLAYER_MOVING_LEFT; }
    else if (player_char == PLAYER_MOVING_UP) { player_y--; trainer_char = PLAYER_MOVING_DOWN; }
    else if (player_char == PLAYER_MOVING_DOWN) { player_y++; trainer_char = PLAYER_MOVING_UP; }

    for (int i = 0; i < num_selectables; i++) {

        if (player_x == selectables[i].x && player_y == selectables[i].y) {
            if (selectables[i].selectable_id == SELECTABLE_TRAINER) { trainer_color = TRAINER_COLOR; is_person = true; }
            else if (selectables[i].selectable_id == SELECTABLE_NPC) { trainer_color = NPC_COLOR; is_person = true; }

            if (is_person) {
                attrset(COLOR_PAIR(trainer_color));
                mvaddch(player_y, player_x, trainer_char); refresh();
                attrset(COLOR_PAIR(DEFAULT_COLOR));
            }
            return &(selectables[i]);
        }
    }
    return &empty_selectable;
}


// Handle a selected selectable
int handle_selected_selectable(int player_x, int player_y, char player_char) {
    Selectable * selectable_ptr;
    Trainer * trainer_ptr;
    NPC * npc_ptr;

    selectable_ptr = get_selectable(player_x, player_y, player_char);
    if (selectable_ptr->selectable_id == SELECTABLE_NONE) return SELECTABLE_CONTINUE_WHILE;
    if (selectable_ptr->selectable_id == SELECTABLE_TRAINER) {
        trainer_ptr = (Trainer *) selectable_ptr->data;
        if (has_battled_trainer(trainer_ptr->id_num)) {
            print_to_message_box("\"We already battled\"");
            return SELECTABLE_CONTINUE_WHILE;
        }
        //Handle trainer battle and return values from that function
        if (battle_trainer(trainer_ptr) != BATTLE_WHITE_OUT) { restore_print_state(); }
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

    return SELECTABLE_BREAK_WHILE;
}


//Erase selectable list
void clear_selectables() {
    num_selectables = 0;
}


//Return a selectable containing a trainer if it is within battle distance of the player
//  x_inc and y_inc will indicate which direction the trainer should move
Selectable * get_triggered_selectable(int player_x, int player_y, int *x_inc, int *y_inc) {
    Selectable * curr_sel;
    int x = 0;
    int y = 0;
    int distance;
    *x_inc = 0;
    *y_inc = 0;

    for (int i = 0; i < num_selectables; i++) {
        curr_sel = &(selectables[i]);
        if (curr_sel->selectable_id == SELECTABLE_TRAINER) {
            Trainer * trainer_ptr = (Trainer *) curr_sel->data;
            if (has_battled_trainer(trainer_ptr->id_num) || ((trainer_ptr->id_num > 200) && (trainer_ptr->id_num < 250))) continue;
            x = curr_sel->x;
            y = curr_sel->y;
            char ch = mvinch(y,x);
            switch (ch) {
                case PLAYER_MOVING_LEFT:
                    *x_inc = -1;
                    *y_inc = 0;
                    distance = x - player_x;
                    if (distance < HORIZONTAL_DISTANCE && distance > 0 && y == player_y) return curr_sel;
                    break;
                case PLAYER_MOVING_RIGHT:
                    *x_inc = 1;
                    *y_inc = 0;
                    distance = player_x - x;
                    if (distance < HORIZONTAL_DISTANCE && distance > 0 && y == player_y) return curr_sel;
                    break;
                case PLAYER_MOVING_UP:
                    *y_inc = -1;
                    *x_inc = 0;
                    distance = y - player_y;
                    if (distance < VERTICAL_DISTANCE && distance > 0 && x == player_x) return curr_sel;
                    break;
                case PLAYER_MOVING_DOWN:
                    *y_inc = 1;
                    *x_inc = 0;
                    distance = player_y - y;
                    if (distance < VERTICAL_DISTANCE && distance > 0 && x == player_x) return curr_sel;
                    break;
                default:
                    print_to_message_box("Found incorrect trainer character");
                    break;
            }
        }
    }
    
    return &empty_selectable;
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
        sprintf(print_str, "Would you like %s to use Cut?\n  Yes\n  No", cut_pok->name);
        print_to_message_box(print_str);
        if (get_selection(MAP_HEIGHT+1, 1, 0) == 1) {
            begin_message_box();
            return 0;
        }

        sprintf(print_str, "%s used Cut!", cut_pok->name);
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