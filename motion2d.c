#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>

#define MOTION_PLAYER_CHARACTER 'P'

void drawBox(int8_t x, int8_t y, int8_t w, int8_t h);

void handle_motion() {
	initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Don't display typed characters
    keypad(stdscr, true); // Enable arrow keys

    clear();

    curs_set(0);

    int x = 10;
    int y = 10;

    mvprintw(22, 1, "Press 'q' to quit");

    
    drawBox(3,3,3,3);


    mvaddch(x, y, MOTION_PLAYER_CHARACTER);

    int ch;
    while ((ch = getch()) != 'q') {
    	mvaddch(x, y, ' '); // Print 'U' at position (0, 0)
        switch (ch) {
        	case KEY_UP:
        		if (mvinch(x-1, y) != ' ') { break; }
                if (x > 0) x--;
                break;
            case KEY_DOWN:
            	if (mvinch(x+1, y) != ' ') { break; }
            	if (x < 20) x++;
                break;
            case KEY_LEFT:
            	if (mvinch(x, y-1) != ' ') { break; }
                if (y > 0) y--;
                break;
            case KEY_RIGHT:
            	if (mvinch(x, y+1) != ' ') { break; }
                if (y < 60) y++;
                break;
        }
        mvaddch(x, y, MOTION_PLAYER_CHARACTER); // Print 'U' at position (0, 0)
    }

    endwin(); // Clean up ncurses

    setvbuf(stdout, NULL, _IOLBF, 0);
}

void drawBox(int8_t x, int8_t y, int8_t w, int8_t h) {
	for (int i = x; i <= x+w; i++) {
		mvaddch(i, y, '|');
		mvaddch(i, y+w, '|');
	}
	for (int i = y; i <= y+h; i++) {
		mvaddch(x, i, '_');
		mvaddch(x+h,i, '_');
	}


}