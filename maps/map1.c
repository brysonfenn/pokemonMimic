#include "map1.h"

#include "ncurses.h"
#include "map_drawing.h"
#include "../player.h"
#include "location.h"


void draw_map1() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Don't display typed characters
    keypad(stdscr, true); // Enable arrow keys
    clear();
    curs_set(0);

    mvprintw(22, 1, "Press 'm' to return to the menu");
    drawBuilding_default(MART_X,MART_Y, "Mart");
    drawBuilding_default(POKE_CENTER_X,POKE_CENTER_Y, "Poke");
    drawBox(MAP_X,MAP_Y,TOWN_WIDTH,TOWN_HEIGHT);  //Draw Town border

    //Draw Doors
    draw_town_exit(MAP_TOP, 10);
    draw_town_exit(MAP_BOTTOM, 10);
    draw_town_exit(MAP_LEFT, 10);
    draw_town_exit(MAP_RIGHT, 10);
}