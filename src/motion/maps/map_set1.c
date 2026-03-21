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
    {MAP_VIR_CITY, "Viridian City", 12, 11, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, true},
    {MAP_R2, "Route 1", 12, 10, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R1, "Route 2", 12, 12, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_STARTER_TOWN, "Starter Town", 12, 13, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, true},
    {MAP_LAB, "Starter Town", 0, 0, MAP_STARTER_TOWN, 0, 0, 0, false},
    {MAP_VIR_FOREST, "Viridian Forest", 12, 8, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_PEW_CITY, "Pewter City", 12, 7, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, true},
    {MAP_GYM1, "Pewter City", 0, 0, MAP_PEW_CITY, 0, 0, 0, false},
    {MAP_R3, "Route 3", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MT_MOON_N, "Mt. Moon", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_CER_CITY, "Cerulean City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM2, "Cerulean City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R4, "Route 4", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_UNDERGROUND, "Underground", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_VERM_CITY, "Vermillion City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R5, "Route 5", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SS_ANNE1, "Vermillion City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SS_ANNE2, "Vermillion City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM3, "Vermillion City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MT_MOON_S, "Mt. Moon", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R6, "Route 6", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_DIG_CAVE, "Diglett's Cave", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R7, "Route 7", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MUSEUM, "Pewter City", 0, 0, MAP_PEW_CITY, 0, 0, 0, false},
    {MAP_R8, "Route 8", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R9, "Route 9", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_ROCK_TUNNEL_N, "Rock Tunnel", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_ROCK_TUNNEL_S, "Rock Tunnel", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_LAV_TOWN, "Lavender Town", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_CEL_CITY, "Celadon City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R10, "Route 10", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_ROCKET1, "Celadon City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_ROCKET2, "Celadon City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_ROCKET3, "Celadon City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GAME_CORNER, "Celadon City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM4, "Celadon City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_TOWER1, "Lavender Town", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_TOWER2, "Lavender Town", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_TOWER3, "Lavender Town", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R11, "Route 11", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R12, "Route 12", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_FU_CITY, "Fuschia City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM5, "Fuschia City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R13, "Route 13", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R14, "Route 14", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R15, "Route 15", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFARI1, "Fuschia City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFARI2, "Fuschia City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFARI3, "Fuschia City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFARI4, "Fuschia City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R16, "Route 16", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SAFF_CITY, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM6, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_CIN_ISLAND, "Cinnabar Island", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_R17, "Route 17", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM7, "Cinnabar Island", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_GYM8, "Viridian City", 0, 0, MAP_VIR_CITY, 0, 0, 0, false},
    {MAP_SILPH1, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH2, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH3, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH4, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH5, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_SILPH6, "Saffron City", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION1, "Cinnabar Island", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION2, "Cinnabar Island", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION3, "Cinnabar Island", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false},
    {MAP_MANSION4, "Cinnabar Island", 0, 0, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, MAP_GENERIC, false}
};

void handle_region_map() {
    int ch = 0;

    Region_loc region_loc = region_list[player.loc->map];
    
    //If this is a secondary location within a main location, switch to main location
    if (region_loc.x == 0) 
        region_loc = region_list[region_loc.up_map];
    

    draw_big_map("");
    draw_static_elements(MAPS_STATIC_ELEMENTS_REGION_MAP);
    begin_message_box();

    //Find Player location
    // update_values_for_region_map(player.loc->map, &player_x, &player_y, &location_has_poke_center, );


    //Draw Player location AND write player location
    if (region_loc.has_poke_center) {
        attrset(COLOR_PAIR(PLAYER_COLOR));
        draw_box(MAP_X+region_loc.x-1, MAP_Y+region_loc.y-1, 3, 3);
        attrset(COLOR_PAIR(DEFAULT_COLOR));
    }
    else {
        attrset(COLOR_PAIR(PLAYER_COLOR));
        mvaddch(MAP_Y+region_loc.y, MAP_X+region_loc.x, 'v');
        attrset(COLOR_PAIR(DEFAULT_COLOR));
    }
    begin_message_box();
    draw_box(MAP_X,MAP_Y+MAP_HEIGHT,MAP_WIDTH,5); //Draw message box
    draw_big_map(region_loc.map_name);
    

    //Get player input
    flushinp();
    ch = getch();


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
        case MAP_R1:
            *x_val = 12;
            *y_val = 12;
            *poke_center = false;
            sprintf(name, "Route 1");
            break;
        case MAP_VIR_CITY:
        case MAP_GYM8:
            *x_val = 12;
            *y_val = 11;
            *poke_center = true;
            sprintf(name, "Viridian City");
            break;
        case MAP_R2:
            *x_val = 12;
            *y_val = 10;
            *poke_center = false;
            sprintf(name, "Route 2");
            break;
        case MAP_VIR_FOREST:
            *x_val = 12;
            *y_val = 8;
            *poke_center = false;
            sprintf(name, "Virdian Forest");
            break;
        case MAP_PEW_CITY:
            *x_val = 12;
            *y_val = 7;
            *poke_center = true;
            sprintf(name, "Pewter City");
            break;
        case MAP_MUSEUM:
            break;
        case MAP_R3:
            break;
        case MAP_MT_MOON_S:
            break;
        case MAP_MT_MOON_N:
            break;
        case MAP_CER_CITY:
        case MAP_GYM2:
            break;
        case MAP_R4:
            break;
        case MAP_UNDERGROUND:
            break;
        case MAP_R5:
            break;
        case MAP_SS_ANNE1:
        case MAP_SS_ANNE2:
            break;
        case MAP_VERM_CITY:
            break;
        case MAP_GYM3:
            break;
        case MAP_R6:
            break;
        case MAP_DIG_CAVE:
            break;
        case MAP_R7:
            break;
        case MAP_R8:
            break;
        case MAP_R9:
            break;
        case MAP_ROCK_TUNNEL_N:
            break;
        case MAP_ROCK_TUNNEL_S:
            break;
        case MAP_LAV_TOWN:
        case MAP_TOWER1:
        case MAP_TOWER2:
        case MAP_TOWER3:
            break;
        case MAP_R10:
            break;
        case MAP_CEL_CITY:
        case MAP_ROCKET1:
        case MAP_ROCKET2:
        case MAP_ROCKET3:
        case MAP_GYM4:
            break;
        case MAP_R11:
            break;
        case MAP_R12:
            break;
        case MAP_FU_CITY:
        case MAP_GYM5:
            break;
        case MAP_R13:
            break;
        case MAP_R14:
            break;
        case MAP_R15:
            break;
        case MAP_SAFARI1:
        case MAP_SAFARI2:
        case MAP_SAFARI3:
        case MAP_SAFARI4:
            break;
        case MAP_R16:
            break;
        case MAP_SAFF_CITY:
        case MAP_GYM6:
        case MAP_SILPH1:
        case MAP_SILPH2:
        case MAP_SILPH3:
        case MAP_SILPH4:
        case MAP_SILPH5:
        case MAP_SILPH6:
            break;
        case MAP_CIN_ISLAND:
        case MAP_GYM7:
        case MAP_MANSION1:
        case MAP_MANSION2:
        case MAP_MANSION3:
        case MAP_MANSION4:
            break;
        case MAP_R17:
            break;
        default:
            break;
    }
}