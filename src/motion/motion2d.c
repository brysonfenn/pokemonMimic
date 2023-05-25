#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "location.h"
#include "maps.h"
#include "map_drawing.h"
#include "doors.h"
#include "selectables.h"

#include "../battles/wild_pokemon.h"
#include "../print_utils.h"
#include "../print_defines.h"
#include "../player.h"
#include "../battles/trainer.h"

void init_map();

typedef void (*init_map_func) ();
typedef int (*map_actions_func) (int, int);

static init_map_func draw_map;
static map_actions_func map_actions;
static init_map_func grass_map;

static char player_char = PLAYER_MOVING_DOWN;

static char * player_y;
static char * player_x;

bool is_movable_space(int yInc, int xInc);

// Draw the current map to the screen and handle player motion until user returns to the menu
void handle_motion() {
    player_y = &(player.loc->y);
    player_x = &(player.loc->x);
    bool is_trainer = false;
    char print_str[256];

    void * selected_ptr;
    Trainer * trainer_ptr;
    char * message_ptr;

    if (player.loc->map == 0) {
        mvprintw(22, 1, "Got player map as 0", player.loc->map); refresh(); sleep(3);
        return;
    }

    change_map(player.loc->map, *player_x, *player_y);

    int ch;
    char next_char;

    flushinp();
    while ((ch = getch()) != 'm') {
        mvaddch(*player_y, *player_x, ' '); 
        switch (ch) {
        	case KEY_UP:
                if (player_char != PLAYER_MOVING_UP) player_char = PLAYER_MOVING_UP;
                else if (*player_y > 1 && is_movable_space(-1,0)) (*player_y)--;
                break;
            case KEY_DOWN:
                next_char = mvinch(*player_y+1, *player_x);
                if (player_char != PLAYER_MOVING_DOWN) player_char = PLAYER_MOVING_DOWN;
                //If it is a fence, jump it
                else if (next_char == '-') {
                    attrset(COLOR_PAIR(PLAYER_COLOR));
                    mvaddch(++(*player_y), *player_x, player_char); refresh();
                    usleep(300000);
                    mvaddch(++(*player_y), *player_x, player_char); refresh();
                    attrset(COLOR_PAIR(DEFAULT_COLOR));
                    mvaddch((*player_y-1), *player_x, '-');
                    flushinp(); continue;
                }
                //Else just move one down
            	else if (*player_y < 20 && is_movable_space(1,0)) (*player_y)++;
                break;
            case KEY_LEFT:
                if (player_char != PLAYER_MOVING_LEFT) player_char = PLAYER_MOVING_LEFT;
                //Move only one space if next space is surrounded (door);
                else if (*player_x > 1 && !is_movable_space(-1,-1) && !is_movable_space(1,-1) && is_movable_space(0,-1)) 
                    (*player_x)--;
                //Default movement for horizontal is 2 spaces
                else if (*player_x > 1 && is_movable_space(0,-1) && is_movable_space(0,-2)) (*player_x)-=2;
                //Only move one space if there is only one space to move
                else if (*player_x > 1 && is_movable_space(0,-1)) (*player_x)--;
                break;
            case KEY_RIGHT:
                if (player_char != PLAYER_MOVING_RIGHT) player_char = PLAYER_MOVING_RIGHT;
                //Move only one space if next space is surrounded (door);
                else if (*player_x < 100 && !is_movable_space(-1,1) && !is_movable_space(1,1) && is_movable_space(0,1)) 
                    (*player_x)++;
                //Default movement for horizontal is 2 spaces
                else if (*player_x < 100 && is_movable_space(0,1) && is_movable_space(0,2)) (*player_x)+=2;
                //Only move one space if there is only one space to move
                else if (*player_x < 100 && is_movable_space(0,1)) (*player_x)++;
                break;
            case 'a':
                //Redraw player
                attrset(COLOR_PAIR(PLAYER_COLOR)); mvaddch(*player_y, *player_x, player_char); 
                selected_ptr = get_selectable(*player_x, *player_y, player_char, &is_trainer);
                if (is_trainer) {
                    trainer_ptr = (Trainer *) selected_ptr;
                    battle_trainer(trainer_ptr);
                    init_map();
                    continue;
                }
                else {
                    message_ptr = (char *) selected_ptr;
                    if (strcmp(message_ptr,"NONE") == 0) continue;
                    mvprintw(21,MAP_X,message_ptr);
                }
                break;
        }
        grass_map();

        bool hitGrass = ((mvinch(*player_y, *player_x) & A_CHARTEXT) == GRASS_CHAR);
        int random = rand() % 100;

        // Set player color, move, and unset
        attrset(COLOR_PAIR(PLAYER_COLOR));
        mvaddch(*player_y, *player_x, player_char);
        attrset(COLOR_PAIR(DEFAULT_COLOR));

        //Display player location
        // sprintf(print_str, "Player location (%d,%d)", *player_x, *player_y);
        // mvprintw(21,4, print_str); 

        refresh();

        if (hitGrass && (random < 10)) {
            blink_screen(5, init_map);
            battle_wild_pokemon();
            init_map();
        }

        // If action is executed, reinitialize the map
        if (map_actions(*player_x, *player_y)) {
            init_map();
        }
    }
}

//Change and redraw the map and player
void change_map(int map, int x, int y) {
    clear();
    clear_doors();
    clear_selectables();
    usleep(10000);

    player.loc->x = x;
    player.loc->y = y;
    player.loc->map = map;
    change_map_funcs(map, &draw_map, &map_actions, &grass_map);

    init_map();
}

//Check if a given space movement would result in a collision
bool is_movable_space(int yInc, int xInc) {
    char next_tile = mvinch(*player_y+yInc, *player_x+xInc);
    if (next_tile == ' ' || next_tile == GRASS_CHAR)
        return true;
    else
        return false;
}

//Draw map and player onto the screen
void init_map() {
    clear();
    draw_map();
    draw_box(MAP_X,MAP_Y+MAP_HEIGHT,MAP_WIDTH,5); //Draw message box

    attrset(COLOR_PAIR(DEFAULT_COLOR));
    mvprintw(MAP_Y+MAP_HEIGHT+1, MAP_X+1, "Map: %d", player.loc->map); refresh();

    attrset(COLOR_PAIR(PLAYER_COLOR));
    mvaddch(*player_y, *player_x, player_char);
    attrset(COLOR_PAIR(DEFAULT_COLOR));

    refresh();
}