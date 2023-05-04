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

static init_map_func draw_map;
static map_actions_func map_actions;
static init_map_func grass_map;

static char player_char = MOVING_DOWN;

static char * player_y;
static char * player_x;

bool is_movable_space(int yInc, int xInc);

void handle_motion() {
    player_y = &(player.loc->y);
    player_x = &(player.loc->x);

    if (player.loc->map == 0) {
        mvprintw(22, 1, "Got player map as 0", player.loc->map); refresh(); sleep(3);
        pause_town_drawing();
        return;
    }

    change_map(player.loc->map, *player_x, *player_y);

    // Define color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

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
                else if (*player_x > 1 && is_movable_space(0,-1)) (*player_x)--;
                break;
            case KEY_RIGHT:
                if (player_char != MOVING_RIGHT) player_char = MOVING_RIGHT;
                else if (*player_x < 100 && is_movable_space(0,1)) (*player_x)++;
                break;
        }
        grass_map();

        bool hitGrass = ((mvinch(*player_y, *player_x) & A_CHARTEXT) == 'M');

        // Set attributes for next character
        attrset(COLOR_PAIR(2));
        mvaddch(*player_y, *player_x, player_char);
        attrset(COLOR_PAIR(1));
        refresh();
        int random = rand() % 100;

        if (hitGrass && (random < 10)) {
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

    mvprintw(23, 1, "Map: %d", player.loc->map); refresh();

    attrset(COLOR_PAIR(2));
    mvaddch(*player_y, *player_x, player_char);
    attrset(COLOR_PAIR(1));
}


void change_map(int map, int x, int y) {
    clear();
    clear_doors();
    usleep(200000);

    switch (map) {
        case 1:
            draw_map = &draw_map1;
            map_actions = &actions_map1;
            grass_map = &grass_map1;
            player.loc->x = x;
            player.loc->y = y;
            player.loc->map = 1;
            break;
        case 2:
            draw_map = &draw_map2;
            map_actions = &actions_map2;
            grass_map = &grass_map2;
            player.loc->x = x;
            player.loc->y = y;
            player.loc->map = 2;
            break;
    }

    init_map();
}