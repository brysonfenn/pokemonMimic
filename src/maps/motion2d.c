#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "../player.h"
#include "location.h"
#include "maps.h"
#include "map_drawing.h"
#include "../battles/wild_pokemon.h"
#include "../print_utils.h"
#include "doors.h"

#define MOVING_DOWN 'v'
#define MOVING_UP '^'
#define MOVING_LEFT '<'
#define MOVING_RIGHT '>'

void init_map();

typedef void (*init_map_func) ();
typedef int (*map_actions_func) (int, int);

static init_map_func draw_map;
static map_actions_func map_actions;
static init_map_func grass_map;

static char player_char = MOVING_DOWN;

static char * player_y;
static char * player_x;

bool is_movable_space(int yInc, int xInc);

// Draw the current map to the screen and handle player motion until user returns to the menu
void handle_motion() {
    player_y = &(player.loc->y);
    player_x = &(player.loc->x);

    if (player.loc->map == 0) {
        mvprintw(22, 1, "Got player map as 0", player.loc->map); refresh(); sleep(3);
        return;
    }

    change_map(player.loc->map, *player_x, *player_y);

    int ch;

    flushinp();
    while ((ch = getch()) != 'm') {
    	mvaddch(*player_y, *player_x, ' '); 
        switch (ch) {
        	case KEY_UP:
                if (player_char != MOVING_UP) player_char = MOVING_UP;
                else if (*player_y > 1 && is_movable_space(-1,0)) (*player_y)--;
                break;
            case KEY_DOWN:
                if (player_char != MOVING_DOWN) player_char = MOVING_DOWN;
            	else if (*player_y < 20 && is_movable_space(1,0)) (*player_y)++;
                break;
            case KEY_LEFT:
                if (player_char != MOVING_LEFT) player_char = MOVING_LEFT;
                //Move only one space if next space is surrounded (door);
                else if (*player_x > 1 && !is_movable_space(-1,-1) && !is_movable_space(1,-1) && is_movable_space(0,-1)) 
                    (*player_x)--;
                //Default movement for horizontal is 2 spaces
                else if (*player_x > 1 && is_movable_space(0,-1) && is_movable_space(0,-2)) (*player_x)-=2;
                //Only move one space if there is only one space to move
                else if (*player_x > 1 && is_movable_space(0,-1)) (*player_x)--;
                break;
            case KEY_RIGHT:
                if (player_char != MOVING_RIGHT) player_char = MOVING_RIGHT;
                //Move only one space if next space is surrounded (door);
                else if (*player_x < 100 && !is_movable_space(-1,1) && !is_movable_space(1,1) && is_movable_space(0,1)) 
                    (*player_x)++;
                //Default movement for horizontal is 2 spaces
                else if (*player_x < 100 && is_movable_space(0,1) && is_movable_space(0,2)) (*player_x)+=2;
                //Only move one space if there is only one space to move
                else if (*player_x < 100 && is_movable_space(0,1)) (*player_x)++;
                break;
        }
        grass_map();

        bool hitGrass = ((mvinch(*player_y, *player_x) & A_CHARTEXT) == 'M');

        // Set attributes for next character
        attrset(COLOR_PAIR(PLAYER_COLOR));
        mvaddch(*player_y, *player_x, player_char);
        attrset(COLOR_PAIR(DEFAULT_COLOR));
        refresh();
        int random = rand() % 100;

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
    usleep(200000);

    player.loc->x = x;
    player.loc->y = y;
    player.loc->map = map;
    change_map_funcs(map, &draw_map, &map_actions, &grass_map);

    init_map();
}

//Check if a given space movement would result in a collision
bool is_movable_space(int yInc, int xInc) {
    char next_tile = mvinch(*player_y+yInc, *player_x+xInc);
    if (next_tile == ' ' || next_tile == 'M') {
        return true;
    }
    else {
        return false;
    }
}

//Draw map and player onto the screen
void init_map() {
    draw_map();

    attrset(COLOR_PAIR(DEFAULT_COLOR));

    mvprintw(23, 1, "Map: %d", player.loc->map); refresh();

    attrset(COLOR_PAIR(PLAYER_COLOR));
    mvaddch(*player_y, *player_x, player_char);
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}