#include "map_drawing.h"

#include <ncurses.h>

#include "location.h"
#include "doors.h"
#include "../print_utils.h"

//Draw a house-sized building with a name, door at the bottom-center, and an action attached to the door
void drawBuilding_default(int x, int y, const char* name, int action) { 
    drawBuilding(x,y,DEFAULT_BUILDING_WIDTH,DEFAULT_BUILDING_HEIGHT,name,action); 
}

//Draw a custom-sized building with a name, door at the bottom-center, and an action attached to the door
void drawBuilding(int x, int y, int w, int h, const char* name, int action) {
    drawBox(x,y,w,h);

    // Print the string (max size 6) inside the box
    if (strlen(name) <= 6) {
        mvprintw(y+1, x+1, "%s", name);
    }

    // Draw a door at the bottom of the box
    mvaddch(y + h - 1, x + (w / 2), ' ');  // Door frame
    mvaddch(y + h - 1, x + (w / 2) + 1, ' ');  // Door frame
    mvaddch(y + h - 1, x + (w / 2) - 1, ACS_LRCORNER);
    mvaddch(y + h - 1, x + (w / 2) + 2, ACS_LLCORNER);

    if (action != -1) add_door(x+ (w/2), y+h-1, action, 1);
    
}

//Draw an exit on a given side of the map at a given position
void draw_town_exit(int side, int position) {
    int min_x = MAP_X + position;
    int min_y = MAP_Y + position;
    int max_x = MAP_WIDTH + MAP_X - 1;
    int max_y = MAP_HEIGHT + MAP_Y - 1;

    switch (side) {
        case MAP_TOP:
            mvaddch(MAP_Y, min_x-1, ACS_LRCORNER);  // Bottom-right corner
            mvaddch(MAP_Y, min_x, ' ');
             mvaddch(MAP_Y, min_x+1, ' ');
            mvaddch(MAP_Y, min_x+2, ACS_LLCORNER);  // Bottom-left corner
            break;
        case MAP_BOTTOM:
            mvaddch(max_y, min_x-1, ACS_URCORNER);  // Top-right corner
            mvaddch(max_y, min_x, ' ');
            mvaddch(max_y, min_x+1, ' ');
            mvaddch(max_y, min_x+2, ACS_ULCORNER);  // Top-left corner
            break;
        case MAP_RIGHT:
            mvaddch(min_y-1, max_x, ACS_LLCORNER);  // Bottom-left corner
            mvaddch(min_y, max_x, ' ');
            mvaddch(min_y+1, max_x, ACS_ULCORNER);  // Top-left corner
            break;
        case MAP_LEFT:
            mvaddch(min_y-1, MAP_X, ACS_LRCORNER);  // Bottom-right corner
            mvaddch(min_y, MAP_X, ' ');
            mvaddch(min_y+1, MAP_X, ACS_URCORNER);  // Top-right corner
            break;
    }
}

void blink_screen(int num_times, void (*draw_map_func) ()) {
    usleep(200000);
    for (int i = 0; i < num_times; i++) {
        clear();
        refresh();
        usleep(100000);
        draw_map_func();
        refresh();
        usleep(100000);
    }
}