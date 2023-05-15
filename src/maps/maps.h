#ifndef MAPS_H
#define MAPS_H

void change_map_funcs(int map_num, void (**draw_map)(), int (**actions) (int, int), void (**grass_map)());

void back_to_poke_center();

#endif //MAP1_H