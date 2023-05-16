#ifndef MAP_DRAWING_H
#define MAP_DRAWING_H

#define DEFAULT_BUILDING_WIDTH 9
#define DEFAULT_BUILDING_HEIGHT 3

#define MAP_WIDTH 58
#define MAP_HEIGHT 20
#define MAP_X 3
#define MAP_Y 1

#define DEFAULT_COLOR 1
#define PLAYER_COLOR 2
#define GRASS_COLOR 3

#define GRASS_CHAR 'M'

//Sides of map - used to call "draw_town_exit()"
enum town_side { MAP_TOP, MAP_BOTTOM, MAP_LEFT, MAP_RIGHT };


//Functions

//Draw a house-sized building with a door and an action attached
void drawBuilding_default(int x, int y, const char* str, int action);

//Draw a custom-sized building with a door and an action attached
void drawBuilding(int x, int y, int w, int h, const char* str, int action);

//Draw an exit on a given side of the map at a given position
void draw_town_exit(int side, int position);

//Blink screen a given number of times, pass in a map function to blink
void blink_screen(int num_times, void (*draw_map_func) ());

#endif //MAP_DRAWING_H