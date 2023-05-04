#ifndef MAP_DRAWING_H
#define MAP_DRAWING_H

#define DEFAULT_BUILDING_WIDTH 9
#define DEFAULT_BUILDING_HEIGHT 3

#define TOWN_WIDTH 60
#define TOWN_HEIGHT 20
#define MAP_X 10
#define MAP_Y 1

enum town_side { MAP_TOP, MAP_BOTTOM, MAP_LEFT, MAP_RIGHT };

void drawBuilding_default(int x, int y, const char* str, int action);

//Includes adding door
void drawBuilding(int x, int y, int w, int h, const char* str, int action);

void draw_town_exit(int x, int y);

void pause_town_drawing();

void blink_screen(int num_times, void (*func) ());

#endif //MAP_DRAWING_H