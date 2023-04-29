#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>

#include "items.h"
#include "maps/location.h"
#include "maps/map1.h"
#include "maps/map_drawing.h"

#define MOTION_PLAYER_CHARACTER 'P'

void pause_town_drawing();
void init_map();

typedef void (*init_map_func) ();
typedef int (*map_actions_func) (int, int);

init_map_func draw_map = &draw_map1;
map_actions_func map_actions = &actions_map1;

void handle_motion() {
    char * player_y = &(player.loc->y);
    char * player_x = &(player.loc->x);
	
    init_map();

    int ch;
    while ((ch = getch()) != 'm') {
    	mvaddch(*player_y, *player_x, ' '); 
        switch (ch) {
        	case KEY_UP:
        		if (mvinch(*player_y-1, *player_x) != ' ') { break; }
                if (*player_y > 1) (*player_y)--;
                break;
            case KEY_DOWN:
            	if (mvinch(*player_y+1, *player_x) != ' ') { break; }
            	if (*player_y < 20) (*player_y)++;
                break;
            case KEY_LEFT:
            	if (mvinch(*player_y, *player_x-1) != ' ') { break; }
                if (*player_x > 1) (*player_x)--;
                break;
            case KEY_RIGHT:
            	if (mvinch(*player_y, *player_x+1) != ' ') { break; }
                if (*player_x < 100) (*player_x)++;
                break;
        }
        mvaddch(*player_y, *player_x, MOTION_PLAYER_CHARACTER); 
        refresh();

        // If we performed an action, reinitialize the map
        if (map_actions(*player_x, *player_y)) {
            init_map();
        }
    }

    pause_town_drawing();
}

void init_map() {
    draw_map();
    mvaddch(player.loc->y, player.loc->x, MOTION_PLAYER_CHARACTER);
}