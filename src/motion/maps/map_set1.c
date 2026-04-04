#include "map_set1.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../maps.h"
#include "../selectables.h"
#include "../../print/print_utils.h"

#include "../../battles/trainer.h"
#include "../../monsters/pokemon_id_list.h"
#include "../../monsters/pokemon.h"
#include "../../print/print_defines.h"
#include "../../player.h"

void update_values_for_region_map(int map, int * x_val, int * y_val, bool * poke_center, char * name);

void draw_starter_town() {
    draw_big_map("Starter Town");
    audio_loop_file("pallet.mp3");

    draw_building_default(MAP_X+6,MAP_Y+2, "Poke", POKE_CENTER_ACTION);
    draw_building_default(MAP_X+30,MAP_Y+5, "LAB", DOORS_NEW_MAP);
    add_building_portal(MAP_X+30, MAP_Y+5, MAP_LAB);
    
    //Draw Doors
    add_connection_to_big_map(MAP_TOP, 41, MAP_R1);
    add_connection_to_big_map(MAP_BOTTOM, 10, MAP_R17);

    add_surf_selectable(MAP_X+10, MAP_Y+11);

    refresh();
}


void draw_route1() {
    draw_big_map("Route 1");
    audio_loop_file("route1.mp3");
    add_connection_to_big_map(MAP_TOP, 25, MAP_VIR_CITY);
    add_connection_to_big_map(MAP_BOTTOM, 41, MAP_STARTER_TOWN);
}


void draw_lab() {
    draw_interior(MAP_X+30, MAP_Y+5, MAP_STARTER_TOWN);
    begin_message_box();
    print_to_message_box("Professor's Lab");
    audio_loop_file("pallet.mp3");

    add_trainer_by_id(INTERIOR_X+10, INTERIOR_Y+3, 251, PLAYER_MOVING_DOWN);
}




typedef struct Region_loc {
    Map_id map_id;
    char map_name[32];
    char x;
    char y;
    char up_map;
    char right_map;
    char down_map;
    char left_map;
    bool has_poke_center;
} Region_loc;

Region_loc region_list[MAX_MAP_NUM+1] = {
    {MAP_GENERIC, "LOCATION NOT FOUND", 0, 0, MAP_GENERIC, 0, 0, 0, false},
    {MAP_VIR_CITY, "Viridian City", 12, 11, MAP_R2, MAP_GENERIC, MAP_R1, MAP_GENERIC, true},
    {MAP_R2, "Route 2", 12, 10, MAP_VIR_FOREST, MAP_GENERIC, MAP_VIR_CITY, MAP_GENERIC, false},
    {MAP_R1, "Route 1", 12, 12, MAP_VIR_CITY, MAP_GENERIC, MAP_STARTER_TOWN, MAP_GENERIC, false},
    {MAP_STARTER_TOWN, "Starter Town", 12, 13, MAP_R1, MAP_GENERIC, MAP_R17, MAP_GENERIC, true},
    {MAP_LAB, "LOCATION NOT FOUND", 0, 0, MAP_STARTER_TOWN, 0, 0, 0, false},
    {MAP_VIR_FOREST, "Viridian Forest", 12, 8, MAP_PEW_CITY, MAP_R7, MAP_R2, MAP_GENERIC, false},
    {MAP_PEW_CITY, "Pewter City", 12, 7, MAP_GENERIC, MAP_R3, MAP_VIR_FOREST, MAP_GENERIC, true},
    {MAP_GYM1, "LOCATION NOT FOUND", 0, 0, MAP_PEW_CITY, 0, 0, 0, false},
    {MAP_R3, "Route 3", 18, 7, MAP_MT_MOON_S, MAP_GENERIC, MAP_GENERIC, MAP_PEW_CITY, true},
    {MAP_MT_MOON_N, "Mt. Moon", 24, 4, MAP_GENERIC, MAP_CER_CITY, MAP_GENERIC, MAP_MT_MOON_S, false},
    {MAP_CER_CITY, "Cerulean City", 36, 4, MAP_R4, MAP_R8, MAP_SAFF_CITY, MAP_MT_MOON_N, true},
    {MAP_GYM2, "LOCATION NOT FOUND", 0, 0, MAP_CER_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R4, "Route 4", 36, 1, MAP_GENERIC, MAP_R5, MAP_CER_CITY, MAP_GENERIC, false},
    {MAP_UNDERGROUND, "Underground", 36, 7, MAP_CER_CITY, MAP_LAV_TOWN, MAP_VERM_CITY, MAP_CEL_CITY, false},
    {MAP_VERM_CITY, "Vermillion City", 36, 11, MAP_SAFF_CITY, MAP_R6, MAP_VERM_CITY, MAP_VERM_CITY, true},
    {MAP_R5, "Route 5", 42, 1, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_R4, false},
    {MAP_SS_ANNE1, "LOCATION NOT FOUND", 0, 0, MAP_VERM_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SS_ANNE2, "LOCATION NOT FOUND", 0, 0, MAP_VERM_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM3, "LOCATION NOT FOUND", 0, 0, MAP_VERM_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MT_MOON_S, "Mt. Moon", 18, 4, MAP_MT_MOON_N, MAP_MT_MOON_N, MAP_R3, MAP_GENERIC, false},
    {MAP_R6, "Route 6", 40, 11, MAP_DIG_CAVE, MAP_R10, MAP_GENERIC, MAP_VERM_CITY, false},
    {MAP_DIG_CAVE, "Diglett's Cave", 29, 10, MAP_R7, MAP_R6, MAP_R6, MAP_R7, false},
    {MAP_R7, "Route 7", 17, 11, MAP_VIR_FOREST, MAP_DIG_CAVE, MAP_DIG_CAVE, MAP_VIR_FOREST, false},
    {MAP_MUSEUM, "LOCATION NOT FOUND", 0, 0, MAP_PEW_CITY, 0, 0, 0, false},
    {MAP_R8, "Route 8", 39, 4, MAP_GENERIC, MAP_R9, MAP_GENERIC, MAP_CER_CITY, false},
    {MAP_R9, "Route 9", 42, 4, MAP_GENERIC, MAP_ROCK_TUNNEL_N, MAP_ROCK_TUNNEL_N, MAP_R8, false},
    {MAP_ROCK_TUNNEL_N, "Rock Tunnel", 45, 4, MAP_R9, MAP_ROCK_TUNNEL_S, MAP_ROCK_TUNNEL_S, MAP_R9, true},
    {MAP_ROCK_TUNNEL_S, "Rock Tunnel", 45, 5, MAP_ROCK_TUNNEL_N, MAP_LAV_TOWN, MAP_LAV_TOWN, MAP_ROCK_TUNNEL_N, false},
    {MAP_LAV_TOWN, "Lavender Town", 45, 7, MAP_ROCK_TUNNEL_S, MAP_GENERIC, MAP_R10, MAP_SAFF_CITY, true},
    {MAP_CEL_CITY, "Celadon City", 27, 7, MAP_GENERIC, MAP_SAFF_CITY, MAP_GENERIC, MAP_R15, true},
    {MAP_R10, "Route 10", 45, 11, MAP_LAV_TOWN, MAP_GENERIC, MAP_R11, MAP_R6, false},
    {MAP_ROCKET1, "LOCATION NOT FOUND", 0, 0, MAP_CEL_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_ROCKET2, "LOCATION NOT FOUND", 0, 0, MAP_CEL_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_ROCKET3, "LOCATION NOT FOUND", 0, 0, MAP_CEL_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GAME_CORNER, "LOCATION NOT FOUND", 0, 0, MAP_CEL_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM4, "LOCATION NOT FOUND", 0, 0, MAP_CEL_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_TOWER1, "LOCATION NOT FOUND", 0, 0, MAP_LAV_TOWN, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_TOWER2, "LOCATION NOT FOUND", 0, 0, MAP_LAV_TOWN, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_TOWER3, "LOCATION NOT FOUND", 0, 0, MAP_LAV_TOWN, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R11, "Route 11", 45, 14, MAP_R10, MAP_GENERIC, MAP_GENERIC, MAP_R12, false},
    {MAP_R12, "Route 12", 35, 14, MAP_GENERIC, MAP_R11, MAP_GENERIC, MAP_FU_CITY, false},
    {MAP_FU_CITY, "Fuschia City", 29, 14, MAP_R13, MAP_R12, MAP_R16, MAP_GENERIC, true},
    {MAP_GYM5, "LOCATION NOT FOUND", 0, 0, MAP_FU_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R13, "Route 13", 23, 13, MAP_R14, MAP_FU_CITY, MAP_FU_CITY, MAP_GENERIC, false},
    {MAP_R14, "Route 14", 23, 10, MAP_R15, MAP_GENERIC, MAP_R13, MAP_GENERIC, false},
    {MAP_R15, "Route 15", 23, 7, MAP_GENERIC, MAP_CEL_CITY, MAP_R14, MAP_GENERIC, false},
    {MAP_SAFARI1, "LOCATION NOT FOUND", 0, 0, MAP_FU_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFARI2, "LOCATION NOT FOUND", 0, 0, MAP_FU_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFARI3, "LOCATION NOT FOUND", 0, 0, MAP_FU_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFARI4, "LOCATION NOT FOUND", 0, 0, MAP_FU_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R16, "Route 16", 21, 16, MAP_FU_CITY, MAP_FU_CITY, MAP_GENERIC, MAP_CIN_ISLAND, false},
    {MAP_SAFF_CITY, "Saffron City", 36, 7, MAP_CER_CITY, MAP_LAV_TOWN, MAP_VERM_CITY, MAP_CEL_CITY, true},
    {MAP_GYM6, "LOCATION NOT FOUND", 0, 0, MAP_SAFF_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_CIN_ISLAND, "Cinnabar Island", 12, 15, MAP_R17, MAP_R16, MAP_R16, MAP_GENERIC, true},
    {MAP_R17, "Route 17", 12, 14, MAP_STARTER_TOWN, MAP_GENERIC, MAP_CIN_ISLAND, MAP_GENERIC, false},
    {MAP_GYM7, "LOCATION NOT FOUND", 0, 0, MAP_CIN_ISLAND, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM8, "LOCATION NOT FOUND", 0, 0, MAP_VIR_CITY, 0, 0, 0, false},
    {MAP_SILPH1, "LOCATION NOT FOUND", 0, 0, MAP_SAFF_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH2, "LOCATION NOT FOUND", 0, 0, MAP_SAFF_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH3, "LOCATION NOT FOUND", 0, 0, MAP_SAFF_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH4, "LOCATION NOT FOUND", 0, 0, MAP_SAFF_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH5, "LOCATION NOT FOUND", 0, 0, MAP_SAFF_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH6, "LOCATION NOT FOUND", 0, 0, MAP_SAFF_CITY, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION1, "LOCATION NOT FOUND", 0, 0, MAP_CIN_ISLAND, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION2, "LOCATION NOT FOUND", 0, 0, MAP_CIN_ISLAND, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION3, "LOCATION NOT FOUND", 0, 0, MAP_CIN_ISLAND, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION4, "LOCATION NOT FOUND", 0, 0, MAP_CIN_ISLAND, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false}
};

void handle_region_map() {
    int ch = 0;

    Region_loc region_loc = region_list[player.loc->map];

    while (1) {
        clear();
        //If this is a secondary location within a main location, switch to main location
        if (region_loc.x == 0) 
            region_loc = region_list[region_loc.up_map];
        
        draw_big_map("");
        draw_static_elements(MAPS_STATIC_ELEMENTS_REGION_MAP);
        print_btn_instructions(false);
        begin_message_box();

        //Draw location indicator AND write player location in message box
        if (region_loc.has_poke_center) {
            attrset(COLOR_PAIR(PLAYER_COLOR));
            draw_box(MAP_X+region_loc.x-1, MAP_Y+region_loc.y-1, 3, 3);
            attrset(COLOR_PAIR(DEFAULT_COLOR));
        }
        else {
            attrset(COLOR_PAIR(PLAYER_COLOR));
            mvaddch(MAP_Y+region_loc.y, MAP_X+region_loc.x, '*');
            attrset(COLOR_PAIR(DEFAULT_COLOR));
        }
        begin_message_box();
        draw_box(MAP_X,MAP_Y+MAP_HEIGHT,MAP_WIDTH,5); //Draw message box
        draw_big_map(region_loc.map_name);
        

        //Get player input
        flushinp();
        ch = getch();

        if (ch == CANCEL_CHAR || ch == CANCEL_CHAR_2) break;
        else if (ch == KEY_UP && (region_list[region_loc.up_map].map_id != MAP_GENERIC)) {
            region_loc = region_list[region_loc.up_map];
        }
        else if (ch == KEY_RIGHT && (region_list[region_loc.right_map].map_id != MAP_GENERIC)) {
            region_loc = region_list[region_loc.right_map];
        }
        else if (ch == KEY_DOWN && (region_list[region_loc.down_map].map_id != MAP_GENERIC)) {
            region_loc = region_list[region_loc.down_map];
        }
        else if (ch == KEY_LEFT && (region_list[region_loc.left_map].map_id != MAP_GENERIC)) {
            region_loc = region_list[region_loc.left_map];
        }
        
    }
    
}

void update_values_for_region_map(int map, int * x_val, int * y_val, bool * poke_center, char * name) {
    switch (map) {
        case MAP_STARTER_TOWN:
        case MAP_LAB:
            *x_val = 12;
            *y_val = 13;
            *poke_center = true;
            sprintf(name, "Starter Town");
            break;
        default:
            break;
    }
}