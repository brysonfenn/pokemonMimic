#include "map_drawing.h"

#include <ncurses.h>

#include "location.h"
#include "doors.h"

void drawBuilding_default(int x, int y, const char* str, int action) { drawBuilding(x,y,DEFAULT_BUILDING_WIDTH,DEFAULT_BUILDING_HEIGHT,str,action); }

void drawBox(int x, int y, int w, int h) {
    mvaddch(y, x, ACS_ULCORNER);  // Top-left corner
    mvaddch(y, x + w - 1, ACS_URCORNER);  // Top-right corner
    mvaddch(y + h - 1, x, ACS_LLCORNER);  // Bottom-left corner
    mvaddch(y + h - 1, x + w - 1, ACS_LRCORNER);  // Bottom-right corner

    for (int i = x + 1; i < x + w - 1; i++) {
        mvaddch(y, i, ACS_HLINE);  // Top and bottom edges
        mvaddch(y + h - 1, i, ACS_HLINE);
    }

    for (int i = y + 1; i < y + h - 1; i++) {
        mvaddch(i, x, ACS_VLINE);  // Left and right edges
        mvaddch(i, x + w - 1, ACS_VLINE);
    }
}

void draw_town_exit(int side, int position) {
    int min_x = MAP_X + position;
    int min_y = MAP_Y + position;
    int max_x = TOWN_WIDTH + MAP_X - 1;
    int max_y = TOWN_HEIGHT + MAP_Y - 1;

    switch (side) {
        case MAP_TOP:
            mvaddch(MAP_Y, min_x-1, ACS_LRCORNER);  // Bottom-right corner
            mvaddch(MAP_Y, min_x, ' ');
            mvaddch(MAP_Y, min_x+1, ACS_LLCORNER);  // Bottom-left corner
            break;
        case MAP_BOTTOM:
            mvaddch(max_y, min_x-1, ACS_URCORNER);  // Top-right corner
            mvaddch(max_y, min_x, ' ');
            mvaddch(max_y, min_x+1, ACS_ULCORNER);  // Top-left corner
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

// Draw a building, max string size is 6
void drawBuilding(int x, int y, int w, int h, const char* str, int action) {
    drawBox(x,y,w,h);

    // Print the string (max size 6) inside the box
    if (strlen(str) <= 6) {
        mvprintw(y+1, x+1, "%s", str);
    }

    // Draw a door at the bottom of the box
    mvaddch(y + h - 1, x + (w / 2), ' ');  // Door frame
    add_door(x+ (w/2), y+h-1, action);
    mvaddch(y + h - 1, x + (w / 2) - 1, ACS_LRCORNER);
    mvaddch(y + h - 1, x + (w / 2) + 1, ACS_LLCORNER);
}

void pause_town_drawing() {
    endwin(); // Clean up ncurses
    setvbuf(stdout, NULL, _IOLBF, 0);
}