#ifndef MAP_DRAWING_H
#define MAP_DRAWING_H

#define DEFAULT_BUILDING_WIDTH 9
#define DEFAULT_BUILDING_HEIGHT 3

#define MAP_WIDTH 58
#define MAP_HEIGHT 18
#define MAP_X 3
#define MAP_Y 1

#define INTERIROR_X 13
#define INTERIOR_Y 2
#define INTERIOR_WIDTH 28
#define INTERIOR_HEIGHT 8

#define MART_ACTION 1
#define POKE_CENTER_ACTION 2

#define GRASS_CHAR 'W'

//Sides of map - used to call "draw_town_exit()"
enum town_side { MAP_TOP, MAP_BOTTOM, MAP_LEFT, MAP_RIGHT };


//Functions

//Draw a house-sized building with a door and an action attached
void drawBuilding_default(int x, int y, const char* str, int action);

//Draw a custom-sized building with a door and an action attached
void drawBuilding(int x, int y, int w, int h, const char* str, int action);

//Draw building interior
void draw_interior(int exit_map_x, int exit_map_y, int exit_map);

//Draw an exit on a given side of the map at a given position
void draw_town_exit(int side, int position);

//Blink screen a given number of times, pass in a map function to blink
void blink_screen(int num_times, void (*draw_map_func) ());

#endif //MAP_DRAWING_H