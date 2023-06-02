#include "maps.h"

#include <ncurses.h>

#include "maps/generic_map.h"
#include "maps/map1.h"
#include "maps/map2.h"
#include "maps/map3.h"
#include "maps/map4.h"
#include "maps/building1.h"
#include "maps/map_vir_forest.h"

#include "map_drawing.h"
#include "motion2d.h"
#include "location.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"

//Don't draw any grass for building maps
void no_grass() {}

//Map functions
static void (*draw_funcs[10])() = { &draw_generic_map, &draw_map1, &draw_map2, &draw_map3, &draw_map4, &draw_b1, &draw_vir_forest };

static void (*grass_funcs[10])() = { &grass_generic_map, &grass_map1, &grass_map2, &grass_map3, &grass_map4, &no_grass, &grass_vir_forest };


void draw_map_from_file() {
    FILE *file = fopen("map_sample.txt", "r");  // Open the file for reading

    if (file == NULL) {
        printw("Failed to map file.");
        return;
    }

    int ch;
    char ch8;
    int col_num = 0;
    int line_num = 0;

    //First line has nothing of value
    while ((ch = fgetc(file)) != '\n') { }

    move(line_num+MAP_Y, MAP_X);

    while ((ch = fgetc(file)) != EOF) {
        ch8 = ch & A_CHARTEXT;

        if (ch8 == '\n') {
            line_num++;
            move(line_num+MAP_Y, MAP_X-1);
            col_num = 0;
        }

        if (col_num < 3) {
            col_num++;
            continue;
        }

        if (ch8 == 'Y' || ch8 == 'W') attrset(COLOR_PAIR(GRASS_COLOR));

        printw("%c", ch);

        attrset(COLOR_PAIR(DEFAULT_COLOR));
    }
    refresh();

    fclose(file);  // Close the file
}



void change_map_funcs(int map_num, void (**draw_map)(), void (**grass_map)() ) {
    //If map is outside the range, choose generic map functions
    if (map_num < 1 || map_num > 6) {
        *draw_map = &draw_generic_map;
        *grass_map = &grass_generic_map;
        return;
    }

    *draw_map = draw_funcs[map_num];
    *grass_map = grass_funcs[map_num];
}


void back_to_poke_center() {
    begin_list();
    print_to_list("B scurried back to the Pokemon Center..."); sleep(2);
    heal_party();
    // change_map(player.blackout_center->map, POKE_CENTER_X+4, POKE_CENTER_Y+3);
    change_map(player.blackout_center->map, player.blackout_center->x, player.blackout_center->y);

    sleep(1);
}