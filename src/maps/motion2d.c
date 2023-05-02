#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "../player.h"
#include "location.h"
#include "map1.h"
#include "map2.h"
#include "map_drawing.h"
#include "../battles/wild_pokemon.h"
#include "../print_utils.h"
#include "doors.h"

#define MOVING_DOWN 'v'
#define MOVING_UP '^'
#define MOVING_LEFT '<'
#define MOVING_RIGHT '>'

void pause_town_drawing();
void init_map();

typedef void (*init_map_func) ();
typedef int (*map_actions_func) (int, int);

static init_map_func draw_map = &draw_map1;
static map_actions_func map_actions = &actions_map1;
static init_map_func grass_map = &grass_map1;

static char player_char = MOVING_DOWN;

static char * player_y;
static char * player_x;

bool is_movable_space(int yInc, int xInc);

void handle_motion() {
    player_y = &(player.loc->y);
    player_x = &(player.loc->x);

    resume_ncurses();
	
    init_map();

    int ch;
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
                else if (*player_x > 1 && is_movable_space(0,-1)) (*player_x)--;
                break;
            case KEY_RIGHT:
                if (player_char != MOVING_RIGHT) player_char = MOVING_RIGHT;
                else if (*player_x < 100 && is_movable_space(0,1)) (*player_x)++;
                break;
        }
        grass_map();

        bool hitGrass = (mvinch(*player_y, *player_x) == 'M');

        mvaddch(*player_y, *player_x, player_char);
        refresh();

        if (hitGrass && ((rand() % 100) < 25)) {
            blink_screen(5, init_map);
            pause_town_drawing();
            battle_wild_pokemon();
            init_map();
        }

        // If action is executed, reinitialize the map
        if (map_actions(*player_x, *player_y)) {
            init_map();
        }
    }

    pause_town_drawing();
}

bool is_movable_space(int yInc, int xInc) {
    char next_tile = mvinch(*player_y+yInc, *player_x+xInc);
    if (next_tile == ' ' || next_tile == 'M') {
        return true;
    }
    else {
        return false;
    }
}

void init_map() {
    draw_map();

    mvaddch(player.loc->y, player.loc->x, player_char);
}


void change_map(int map, int x, int y) {
    clear();
    clear_doors();
    sleep(1);

    switch (map) {
        case 1:
            mvprintw(23, 1, "Switched to map 1"); refresh();
            draw_map = &draw_map1;
            map_actions = &actions_map1;
            grass_map = &grass_map1;
            *player_x = x;
            *player_y = y;
            break;
        case 2:
            mvprintw(23, 1, "Switched to map 2"); refresh();
            draw_map = &draw_map2;
            map_actions = &actions_map2;
            grass_map = &grass_map2;
            *player_x = x;
            *player_y = y;
            break;
    }

    init_map();
}