#include "map_drawing.h"

#include <ncurses.h>

#include "location.h"
#include "doors.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"

#define MESSAGE_BOX_X MAP_X
#define MESSAGE_BOX_Y (MAP_Y+MAP_HEIGHT)

static int message_line = 0;

//Clear message box and start at the beginning
void begin_message_box() {
    //Clear box
    int start_y = MESSAGE_BOX_Y+1;
    int start_x = MESSAGE_BOX_X+1;
    for (int i = 0; i < 3; i++) {
        mvprintw(start_y+i, start_x, "                                                        ");
    }
    
    message_line = 0;
}


//Print a message to the bottom box
void print_to_message_box(const char * message_str) {
    if (message_line > 2) begin_message_box();

    mvprintw(MESSAGE_BOX_Y+1 + message_line, MESSAGE_BOX_X+2, message_str);
    refresh();
    message_line++;
}


//Draw a house-sized building with a name, door at the bottom-center, and an action attached to the door
void drawBuilding_default(int x, int y, const char* name, int action) { 
    drawBuilding(x,y,DEFAULT_BUILDING_WIDTH,DEFAULT_BUILDING_HEIGHT,name,action); 
}


//Draw a custom-sized building with a name, door at the bottom-center, and an action attached to the door
void drawBuilding(int x, int y, int w, int h, const char* name, int action) {
    draw_box(x,y,w,h);

    // Print the string (max size 6) inside the box
    if (strlen(name) <= 6) {
        mvprintw(y+1, x+(w/2)-1, "%s", name);
    }

    // Draw a door at the bottom of the box
    mvaddch(y + h - 1, x + (w / 2), ' ');  // Door frame
    mvaddch(y + h - 1, x + (w / 2) + 1, ' ');  // Door frame
    mvaddch(y + h - 1, x + (w / 2) - 1, ACS_LRCORNER);
    mvaddch(y + h - 1, x + (w / 2) + 2, ACS_LLCORNER);

    if (action != -1) add_door(x+ (w/2), y+h-1, action, 1);
    
}


//Draw building interior
void draw_interior(int exit_map_x, int exit_map_y, int exit_map) {
    int min_x = INTERIOR_X+10;
    int max_y = INTERIOR_Y+INTERIOR_HEIGHT-1;

    draw_box(INTERIOR_X,INTERIOR_Y,INTERIOR_WIDTH,INTERIOR_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    mvaddch(max_y, min_x-1, ACS_URCORNER);  // Top-right corner
    mvaddch(max_y, min_x, ' ');
    mvaddch(max_y, min_x+1, ' ');
    mvaddch(max_y, min_x+2, ACS_ULCORNER);  // Top-left corner

    add_portal(min_x, max_y, exit_map_x, exit_map_y, exit_map, true);

    refresh();
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