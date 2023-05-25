#include "selectables.h"

#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../battles/trainer.h"
#include "map_drawing.h"
#include "../print_defines.h"

typedef struct Selectable {
    int x;
    int y;
    bool is_trainer;
    void * data;
} Selectable;

static Selectable selectables[30];
static int num_selectables = 0;


static const char * empty_selectable = "NONE";

//Add a trainer at a given location
void add_trainer(char x, char y, struct Trainer * trainer, char face_direction) {
    Trainer * new_trainer = malloc(sizeof(Trainer));
    (*new_trainer) = (*trainer);

    Selectable new_selectable;
    new_selectable.data = new_trainer;
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
void * get_selectable(int player_x, int player_y, char player_char, bool * is_trainer) {
    *is_trainer = false;
    char trainer_char;

    if (player_char == PLAYER_MOVING_LEFT) { player_x--; trainer_char = PLAYER_MOVING_RIGHT; }
    else if (player_char == PLAYER_MOVING_RIGHT) { player_x++; trainer_char = PLAYER_MOVING_LEFT; }
    else if (player_char == PLAYER_MOVING_UP) { player_y--; trainer_char = PLAYER_MOVING_DOWN; }
    else { player_y++; trainer_char = PLAYER_MOVING_UP; }

    for (int i = 0; i < num_selectables; i++) {
        if (player_x == selectables[i].x && player_y == selectables[i].y) {
            *is_trainer = selectables[i].is_trainer;
            attrset(COLOR_PAIR(TRAINER_COLOR));
            mvaddch(player_y, player_x, trainer_char); refresh();
            attrset(COLOR_PAIR(DEFAULT_COLOR));
            return selectables[i].data;
        }
    }
    return empty_selectable;
}

//Erase selectable list
void clear_selectables() {
    for (int i = 0; i < num_selectables; i++) {
        free(selectables[i].data);
    }
    num_selectables = 0;
}