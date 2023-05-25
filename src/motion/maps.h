#ifndef MAPS_H
#define MAPS_H

//enumerate the map function indexes
typedef enum { MAP_GENERIC, MAP_VIRIDIAN, MAP_R2, MAP_R1, MAP_STARTER_TOWN, MAP_LAB
} Map_id;

void change_map_funcs(int map_num, void (**draw_map)(), void (**grass_map)());

void back_to_poke_center();

#endif //MAP1_H