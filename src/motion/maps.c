#include "maps.h"

#include <ncurses.h>

#include "maps/generic_map.h"
#include "maps/map_set1.h"
#include "maps/map_set2.h"
#include "maps/map_set3.h"
#include "maps/map_set4.h"
#include "maps/map_set5.h"

#include "map_drawing.h"
#include "motion2d.h"
#include "location.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"

//Map functions
static void (*draw_funcs[MAX_MAP_NUM+2])() = { &draw_generic_map, 
                                    &draw_vir_city, &draw_route2, &draw_route1, &draw_starter_town, &draw_lab, 
                                    &draw_vir_forest, &draw_pew_city, &draw_gym1, &draw_route3, &draw_mt_moon,
                                    &draw_cer_city, &draw_gym2, &draw_route4, &draw_underground, &draw_vermillion_city,
                                    &draw_route5, &draw_ss_anne1, &draw_ss_anne2 };

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
        case MAP_PEW_CITY:
            sprintf(map_name, "pew_city");
            break;
        case MAP_R3:
            sprintf(map_name, "route3");
            break;
        case MAP_MT_MOON:
            sprintf(map_name, "mt_moon");
            break;
        case MAP_CER_CITY:
            sprintf(map_name, "cer_city");
            break;
        case MAP_GYM2:
            sprintf(map_name, "gym2");
            break;
        case MAP_R4:
            sprintf(map_name, "route4");
            break;
        case MAP_UG_NS:
            sprintf(map_name, "underground_ns");
            break;
        case MAP_R5:
            sprintf(map_name, "route5");
            break;
        case MAP_SS_ANNE1:
            sprintf(map_name, "ss_anne1");
            break;
        case MAP_SS_ANNE2:
            sprintf(map_name, "ss_anne2");
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
        else if (ch == '~') attrset(COLOR_PAIR(WATER_COLOR));

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
    if (map_num < 1 || map_num > MAX_MAP_NUM) {
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