#ifndef MAPS_H
#define MAPS_H

//enumerate the map function indexes
typedef enum { MAP_GENERIC, 
                MAP_VIRIDIAN, MAP_R2, MAP_R1, MAP_STARTER_TOWN, MAP_LAB, 
                MAP_VIR_FOREST, MAP_PEW_CITY, MAP_GYM1, MAP_R3, MAP_MT_MOON_N,
                MAP_CER_CITY, MAP_GYM2, MAP_R4, MAP_UNDERGROUND, MAP_VERM_CITY, MAP_R5,
                MAP_SS_ANNE1, MAP_SS_ANNE2, MAP_GYM3, MAP_MT_MOON_S, MAP_R6,
                MAP_DIG_CAVE, MAP_R7, MAP_MUSEUM, MAP_R8, MAP_R9, MAP_ROCK_TUNNEL_N,
                MAP_ROCK_TUNNEL_S, MAP_LAV_TOWN, MAP_CEL_CITY, MAP_R10, MAP_ROCKET1,
                MAP_ROCKET2, MAP_ROCKET3, MAP_GAME_CORNER, MAP_GYM4, MAP_TOWER1,
                MAP_TOWER2, MAP_TOWER3, MAP_R11, MAP_R12, MAP_FU_CITY, MAP_GYM5
} Map_id;

//Change when adding a map
#define MAX_MAP_NUM MAP_GYM5

//Draw elements (like grass, trees, etc) according to map text file
void draw_static_elements();

int * get_wild_pok_list();

//Change map drawing function according to map_num
void change_map_funcs(int map_num, void (**draw_map)());

//Notify player of moving back to pokecenter, then move them
void move_player_to_poke_center();

#endif //MAP1_H