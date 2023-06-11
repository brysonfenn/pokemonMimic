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

//Draw elements (like grass, trees, etc) according to map text file
void draw_static_elements() {
    char map_name[32] = "empty_map";

    switch (player.loc->map) {
        case MAP_R1:
            sprintf(map_name, "route1");
            break;
        case MAP_VIRIDIAN:
            sprintf(map_name, "vir_city");
            break;
        case MAP_R2:
            sprintf(map_name, "route2");
            break;
        case MAP_VIR_FOREST:
            sprintf(map_name, "vir_forest");
            break;
        default:
            sprintf(map_name, "empty_map");
            break;
    }

    char file_name[64];
    sprintf(file_name, "maps/%s.txt", map_name);
    FILE *file = fopen(file_name, "r");  // Open the file for reading

    if (file == NULL) {
        printw("Failed to map file.");
        return;
    }

    int ch, y, x;
    char ch8;
    int col_num = 0;
    int line_num = 0;

    //First two line has nothing of value
    while ((ch = fgetc(file)) != '\n') { }
    while ((ch = fgetc(file)) != '\n') { }

    move(line_num+MAP_Y+1, MAP_X);

    while ((ch = fgetc(file)) != EOF) {
        ch8 = ch & A_CHARTEXT;

        //If this is an endline char, move to the next line
        if (ch8 == '\n') {
            line_num++;
            move(line_num+MAP_Y+1, MAP_X);
            col_num = 0;
            continue;
        }

        //Skip first three digits -- used for easier visuals in the text file
        if (col_num < 3) {
            //Last line will start with an f, so break if we see one
            if (ch == 'f') break;
            col_num++;
            continue;
        }

        if (ch == 'W') attrset(COLOR_PAIR(GRASS_COLOR));
        else if (ch == 'Y') attrset(COLOR_PAIR(TREE_COLOR));

         //If this is a vertical line or a space, move cursor forward rather than printing
         // vertical line characters in a text file have the value 130
        if (ch == ' ' || ch == 130) {
            getyx(stdscr, y, x);  // Get the current cursor position
            move(y, x+1);   //Move cursor forward one space rather than printing
        }
        else {
            printw("%c", ch);
        }
        
        attrset(COLOR_PAIR(DEFAULT_COLOR));
    }
    refresh();

    fclose(file);  // Close the file
}


//Change map drawing function according to map_num
void change_map_funcs(int map_num, void (**draw_map)()) {
    //If map is outside the range, choose generic map functions
    if (map_num < 1 || map_num > 6) {
        *draw_map = &draw_generic_map;
        return;
    }
    *draw_map = draw_funcs[map_num];
}

//Notify player of moving back to pokecenter, then move them
void back_to_poke_center() {
    char print_str[256];

    begin_list();
    sprintf(print_str, "%s scurried back to the Pokemon Center...", player.name);
    print_to_list(print_str); sleep(2);
    heal_party();
    // change_map(player.blackout_center->map, POKE_CENTER_X+4, POKE_CENTER_Y+3);
    change_map(player.blackout_center->map, player.blackout_center->x, player.blackout_center->y);

    sleep(1);
}