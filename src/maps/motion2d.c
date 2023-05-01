#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "../player.h"
#include "location.h"
#include "map1.h"
#include "map_drawing.h"

#define MOVING_DOWN 'v'
#define MOVING_UP '^'
#define MOVING_LEFT '<'
#define MOVING_RIGHT '>'

void pause_town_drawing();
void init_map();

typedef void (*init_map_func) ();
typedef int (*map_actions_func) (int, int);

init_map_func draw_map = &draw_map1;
map_actions_func map_actions = &actions_map1;

static char player_char = MOVING_DOWN;

static char * player_y;
static char * player_x;

bool is_movable_space(int yInc, int xInc);

void handle_motion() {
    player_y = &(player.loc->y);
    player_x = &(player.loc->x);
	
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
        mvaddch(*player_y, *player_x, player_char); 
        refresh();

        // If we performed an action, reinitialize the map
        if (map_actions(*player_x, *player_y)) {
            init_map();
        }
    }

    pause_town_drawing();
}

bool is_movable_space(int yInc, int xInc) {
    bool movable = true;

    movable &= (mvinch(*player_y+yInc, *player_x+xInc) == ' ');

    return movable;
}

void init_map() {
    draw_map();
    mvaddch(player.loc->y, player.loc->x, player_char);
}