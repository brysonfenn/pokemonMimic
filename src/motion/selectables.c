#include "selectables.h"

#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../battles/trainer.h"
#include "map_drawing.h"
#include "../print/print_defines.h"
#include "../player.h"

#define HORIZONTAL_DISTANCE 10
#define VERTICAL_DISTANCE 4

static Selectable selectables[30];
static int num_selectables = 0;

const char * empty_string = "NONE";
static Selectable empty_selectable = {0, 0, 0};

//Add a trainer at a given location
void add_trainer(char x, char y, struct Trainer * trainer, char face_direction) {
    //initialize empty selectable
    empty_selectable.data = empty_string;

    // Trainer * new_trainer = malloc(sizeof(Trainer));
    // (*new_trainer) = (*trainer);

    Selectable new_selectable;
    new_selectable.data = trainer;
    new_selectable.x = x;
    new_selectable.y = y;
    new_selectable.is_trainer = true;

    selectables[num_selectables] = new_selectable;
    num_selectables++;

    attrset(COLOR_PAIR(TRAINER_COLOR));
    mvaddch(y,x,face_direction);
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}

//Add message (sign, notification, etc)
void add_selectable_message(char x, char y, char * message) {

}

// Return a door (if there is one) at player location, else return zero-door
Selectable * get_selectable(int player_x, int player_y, char player_char) {
    char trainer_char;

    if (player_char == PLAYER_MOVING_LEFT) { player_x--; trainer_char = PLAYER_MOVING_RIGHT; }
    else if (player_char == PLAYER_MOVING_RIGHT) { player_x++; trainer_char = PLAYER_MOVING_LEFT; }
    else if (player_char == PLAYER_MOVING_UP) { player_y--; trainer_char = PLAYER_MOVING_DOWN; }
    else { player_y++; trainer_char = PLAYER_MOVING_UP; }

    for (int i = 0; i < num_selectables; i++) {

        if (player_x == selectables[i].x && player_y == selectables[i].y) {
            attrset(COLOR_PAIR(TRAINER_COLOR));
            mvaddch(player_y, player_x, trainer_char); refresh();
            attrset(COLOR_PAIR(DEFAULT_COLOR));

            return &(selectables[i]);
        }
    }
    return &empty_selectable;
}

//Erase selectable list
void clear_selectables() {
    for (int i = 0; i < num_selectables; i++) {
        // free(selectables[i].data);
    }
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
        if (curr_sel->is_trainer) {
            Trainer * trainer_ptr = (Trainer *) curr_sel->data;
            if (already_battled_trainer(trainer_ptr->id_num) || trainer_ptr->id_num > 200) continue;
            x = curr_sel->x;
            y = curr_sel->y;
            char ch = mvinch(y,x);
            switch (ch) {
                case PLAYER_MOVING_LEFT:
                    *x_inc = -1;
                    distance = x - player_x;
                    if (distance < HORIZONTAL_DISTANCE && distance > 0 && y == player_y) return curr_sel;
                    break;
                case PLAYER_MOVING_RIGHT:
                    *x_inc = 1;
                    distance = player_x - x;
                    if (distance < HORIZONTAL_DISTANCE && distance > 0 && y == player_y) return curr_sel;
                    break;
                case PLAYER_MOVING_UP:
                    *y_inc = -1;
                    distance = y - player_y;
                    if (distance < VERTICAL_DISTANCE && distance > 0 && x == player_x) return curr_sel;
                    break;
                case PLAYER_MOVING_DOWN:
                    *y_inc = 1;
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