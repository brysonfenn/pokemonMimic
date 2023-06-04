#ifndef MAPS_H
#define MAPS_H

//enumerate the map function indexes
typedef enum { MAP_GENERIC, MAP_VIRIDIAN, MAP_R2, MAP_R1, MAP_STARTER_TOWN, MAP_LAB, MAP_VIR_FOREST
} Map_id;

void draw_static_elements();

void change_map_funcs(int map_num, void (**draw_map)());

void back_to_poke_center();

#endif //MAP1_H