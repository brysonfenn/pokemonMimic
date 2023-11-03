#ifndef MOTION22D_H
#define MOTION22D_H

//Initialize handling motion
void init_motion();

// Draw the current map to the screen and handle player motion until user returns to the menu
void handle_motion();

//Change and redraw the map and player
void change_map(int map, int x, int y);

#endif // MOTION22D_H