#include "map_drawing.h"

#include <ncurses.h>

#include "location.h"
#include "doors.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"

static int message_line = 0;
static char line1[100]; static char line2[100]; static char line3[100];
static char * lines[3] = {line1, line2, line3};
static bool text_in_message_box = false;

//Clear message box and start at the beginning
void begin_message_box() {
    text_in_message_box = false;
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
    char* token;
    char* input = strdup(message_str); //input is mutable version of input str
    begin_message_box();
    text_in_message_box = true;

    fix_list_box_overflow(input);
    int count = 0;

    //Get up to 3 lines of text, then print those lines
    while (count <= 2) {
        if (count == 0) token = strtok(input, "\n");
        else token = strtok(NULL, "\n");
        if (token == NULL) break;
        sprintf(lines[count], "%s", token);
        count++;
    }
    for (int i = 0; i < count; i++) {
        mvprintw(MESSAGE_BOX_Y+1 + i, MESSAGE_BOX_X+2, lines[i]);
    }
    
    //Print the rest of the lines, if there are more
    while ((token = strtok(NULL, "\n")) != NULL) {
        await_user();
        sprintf(lines[0], "%s", lines[1]);
        sprintf(lines[1], "%s", lines[2]);
        sprintf(lines[2], "%s", token);

        begin_message_box();
        mvprintw(MESSAGE_BOX_Y+1 + 0, MESSAGE_BOX_X+2, lines[0]);
        mvprintw(MESSAGE_BOX_Y+1 + 1, MESSAGE_BOX_X+2, lines[1]);
        mvprintw(MESSAGE_BOX_Y+1 + 2, MESSAGE_BOX_X+2, lines[2]);
    }
    refresh();
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


//Draw big map outline and print message to message box
void draw_big_map(const char * map_title) {
    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    begin_message_box();
    print_to_message_box(map_title);
}


//Draw building interior --> Coordinates should be the same as the building's coordinates
void draw_interior(int exit_map_x, int exit_map_y, int exit_map) {
    exit_map_x += 4;
    exit_map_y += 3;

    int min_x = INTERIOR_X+10;
    int max_y = INTERIOR_Y+INTERIOR_HEIGHT-1;

    draw_box(INTERIOR_X,INTERIOR_Y,INTERIOR_WIDTH,INTERIOR_HEIGHT);  //Draw Building border
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


//Add a connection to another city on a given side of the current city
void add_connection_to_big_map(int map_side, int position_on_side, int map_id) {
    draw_town_exit(map_side, position_on_side);
    add_exit_portal(map_side, position_on_side, map_id);
}

//Blink screen a given number of times, pass in a map function to blink
void blink_screen(int num_times, void (*draw_map_func) ()) {
    usleep(150000);
    for (int i = 0; i < num_times; i++) {
        clear();
        refresh();
        usleep(100000);
        draw_map_func();
        refresh();
        usleep(100000);
    }
}

bool is_text_in_message_box() {
    return text_in_message_box;
}