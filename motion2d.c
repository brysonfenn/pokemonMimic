#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>

#include "items.h"

#define DEFAULT_BUILDING_WIDTH 9
#define DEFAULT_BUILDING_HEIGHT 3

#define MOTION_PLAYER_CHARACTER 'P'
#define POKE_CENTER_X 3
#define POKE_CENTER_Y 3
#define MART_X 20
#define MART_Y 3
#define POKE_CENTER_ENTER_X POKE_CENTER_X + (DEFAULT_BUILDING_WIDTH / 2)
#define POKE_CENTER_ENTER_Y POKE_CENTER_Y + DEFAULT_BUILDING_HEIGHT - 1
#define MART_ENTER_X MART_X + (DEFAULT_BUILDING_WIDTH / 2)
#define MART_ENTER_Y MART_Y + DEFAULT_BUILDING_HEIGHT - 1


static int player_x = 10;
static int player_y = 10;

void drawBuilding_default(int x, int y, const char* str);
void drawBuilding(int x, int y, int w, int h, const char* str);
void drawBox(int x, int y, int w, int h);

void init_town_drawing();

void pause_town_drawing();

void handle_motion() {
	
    init_town_drawing();

    int ch;
    while ((ch = getch()) != 'm') {
    	mvaddch(player_y, player_x, ' '); // Print 'U' at position (0, 0)
        switch (ch) {
        	case KEY_UP:
        		if (mvinch(player_y-1, player_x) != ' ') { break; }
                if (player_y > 1) player_y--;
                break;
            case KEY_DOWN:
            	if (mvinch(player_y+1, player_x) != ' ') { break; }
            	if (player_y < 20) player_y++;
                break;
            case KEY_LEFT:
            	if (mvinch(player_y, player_x-1) != ' ') { break; }
                if (player_x > 1) player_x--;
                break;
            case KEY_RIGHT:
            	if (mvinch(player_y, player_x+1) != ' ') { break; }
                if (player_x < 60) player_x++;
                break;
        }
        mvaddch(player_y, player_x, MOTION_PLAYER_CHARACTER); // Print 'U' at position (0, 0)
        refresh();

        //Handle building actions
        if (player_x == POKE_CENTER_ENTER_X && player_y == POKE_CENTER_ENTER_Y) {
            sleep(1);
            pause_town_drawing();
            heal_party(); printParty(); sleep(2); clearTerminal();
            mvaddch(player_y, player_x, ' ');
            player_y += 1;
            init_town_drawing();
        }
        else if (player_x == MART_ENTER_X && player_y == MART_ENTER_Y) {
            sleep(1);
            pause_town_drawing();
            while (handle_mart() == ITEM_FAILURE) { clearTerminal(); }
            clearTerminal();
            mvaddch(player_y, player_x, ' ');
            player_y += 1;
            init_town_drawing();
        }
    }

    pause_town_drawing();
    
}

void drawBuilding_default(int x, int y, const char* str) { drawBuilding(x,y,DEFAULT_BUILDING_WIDTH,DEFAULT_BUILDING_HEIGHT,str); }

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

// Draw a building, max string size is 6
void drawBuilding(int x, int y, int w, int h, const char* str) {
    drawBox(x,h,w,h);

    // Print the string (max size 6) inside the box
    if (strlen(str) <= 6) {
        mvprintw(y+1, x+1, "%s", str);
    }

    // Draw a door at the bottom of the box
    mvaddch(y + h - 1, x + (w / 2), ' ');  // Door frame
    mvaddch(y + h - 1, x + (w / 2) - 1, ACS_LRCORNER);
    mvaddch(y + h - 1, x + (w / 2) + 1, ACS_LLCORNER);
}

void init_town_drawing() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Don't display typed characters
    keypad(stdscr, true); // Enable arrow keys
    clear();
    curs_set(0);

    mvprintw(22, 1, "Press 'm' to return to the menu");
    drawBuilding_default(MART_X,MART_Y, "Mart");
    drawBuilding_default(POKE_CENTER_X,POKE_CENTER_Y, "Poke");
    drawBox(0,0,60,20);  //Draw Town border
    
    mvaddch(player_y, player_x, MOTION_PLAYER_CHARACTER);
}

void pause_town_drawing() {
    endwin(); // Clean up ncurses
    setvbuf(stdout, NULL, _IOLBF, 0);
}