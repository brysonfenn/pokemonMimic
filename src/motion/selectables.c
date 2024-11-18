#include "selectables.h"

#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "npc.h"
#include "map_drawing.h"
#include "../player.h"
#include "../battles/trainer.h"
#include "../battles/trainer_list.h"
#include "../battles/battle.h"
#include "../monsters/pokemon.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../audio/audio_player.h"

#define HORIZONTAL_DISTANCE 10
#define VERTICAL_DISTANCE 4

static Selectable selectables[30];
static int num_selectables = 0;

const char * empty_string = "NONE";
static Selectable empty_selectable = {0, 0, SELECTABLE_NONE};
static char snorlax_bits[2] = { 7,  12 };


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

//Add Cuttable Tree at a location
void add_surf_selectable(char x, char y) {
    Selectable new_selectable;
    new_selectable.data = empty_string;
    new_selectable.x = x;
    new_selectable.y = y;
    new_selectable.selectable_id = SELECTABLE_SURF;
    
    selectables[num_selectables] = new_selectable;
    num_selectables++;

    attrset(COLOR_PAIR(WATER_COLOR));
    mvaddch(y,x,'S'); refresh();
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}

//Add Cuttable Tree at a location
void add_snorlax(char x, char y, char snorlax_id_num) {
    char record_bit_num = snorlax_bits[snorlax_id_num];

    //Do not add snorlax if already fought
    if ((player.record_bits >> record_bit_num) & 0x01) {
        return;
    }

    Selectable new_selectable;
    new_selectable.data = &(snorlax_bits[snorlax_id_num]);
    new_selectable.selectable_id = SELECTABLE_SNORLAX;

    new_selectable.x = x;
    new_selectable.y = y;
    selectables[num_selectables] = new_selectable;
    num_selectables++;
    mvaddch(y,x,'o');
    
    new_selectable.x = x;
    new_selectable.y = y+1;
    selectables[num_selectables] = new_selectable;
    num_selectables++;
    mvaddch(y+1,x,'O'); refresh();
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


//Erase selectable list
void clear_selectables() {
    num_selectables = 0;
}


//Return a selectable containing a trainer if it is within battle distance of the player
//  x_inc and y_inc will indicate which direction the trainer should move
Selectable * get_triggered_selectable(int player_x, int player_y, int *x_inc, int *y_inc, char * next_player_char) {
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
            if (player_has_battled_trainer(trainer_ptr->id_num) || ((trainer_ptr->id_num > 200) && (trainer_ptr->id_num < 250))) continue;
            x = curr_sel->x;
            y = curr_sel->y;
            char ch = mvinch(y,x);
            switch (ch) {
                case PLAYER_MOVING_LEFT:
                    *x_inc = -1;
                    *y_inc = 0;
                    distance = x - player_x;
                    *next_player_char = PLAYER_MOVING_RIGHT;
                    if (distance < HORIZONTAL_DISTANCE && distance > 0 && y == player_y) return curr_sel;
                    break;
                case PLAYER_MOVING_RIGHT:
                    *x_inc = 1;
                    *y_inc = 0;
                    distance = player_x - x;
                    *next_player_char = PLAYER_MOVING_LEFT;
                    if (distance < HORIZONTAL_DISTANCE && distance > 0 && y == player_y) return curr_sel;
                    break;
                case PLAYER_MOVING_UP:
                    *y_inc = -1;
                    *x_inc = 0;
                    distance = y - player_y;
                    *next_player_char = PLAYER_MOVING_DOWN;
                    if (distance < VERTICAL_DISTANCE && distance > 0 && x == player_x) return curr_sel;
                    break;
                case PLAYER_MOVING_DOWN:
                    *y_inc = 1;
                    *x_inc = 0;
                    distance = player_y - y;
                    *next_player_char = PLAYER_MOVING_UP;
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
