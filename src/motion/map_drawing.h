#ifndef MAP_DRAWING_H
#define MAP_DRAWING_H

#define MAP_WIDTH 58
#define MAP_HEIGHT 18
#define MAP_X 3
#define MAP_Y 1

#define INTERIOR_X 13
#define INTERIOR_Y 2
#define INTERIOR_WIDTH 28
#define INTERIOR_HEIGHT 8

#define MESSAGE_BOX_X MAP_X
#define MESSAGE_BOX_Y (MAP_Y+MAP_HEIGHT)

#define DEFAULT_BUILDING_WIDTH 9
#define DEFAULT_BUILDING_HEIGHT 3

#define MART_ACTION 1
#define POKE_CENTER_ACTION 2
#define FOSSIL_ACTION 3
#define FOSSIL_PROCESS_ACTION 4

#define GRASS_CHAR 'W'

//Sides of map - used to call "draw_town_exit()"
enum town_side { MAP_TOP, MAP_BOTTOM, MAP_LEFT, MAP_RIGHT };


//Functions

//Clear message box and start at the beginning
void begin_message_box();

//Print a message to the bottom box
void print_to_message_box(const char * message_str);

//Draw a house-sized building with a door and an action attached
void drawBuilding_default(int x, int y, const char* str, int action);

//Draw a custom-sized building with a door and an action attached
void drawBuilding(int x, int y, int w, int h, const char* str, int action);

//Draw big map outline and print message to message box
void draw_big_map(const char * map_title);

//Draw building interior
void draw_interior(int exit_map_x, int exit_map_y, int exit_map);

//Draw an exit on a given side of the map at a given position
void draw_town_exit(int side, int position);

//Add a connection to another city on a given side of the current city
void add_connection_to_big_map(int map_side, int position_on_side, int map_id);

//Blink screen a given number of times, pass in a map function to blink
void blink_screen(int num_times, void (*draw_map_func) ());

#endif //MAP_DRAWING_H