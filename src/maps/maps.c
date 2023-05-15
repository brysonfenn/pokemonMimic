#include "maps.h"

#include <ncurses.h>

#include "generic_map.h"
#include "map1.h"
#include "map2.h"
#include "map3.h"
#include "motion2d.h"
#include "../player.h"
#include "../print_utils.h"

static void (*draw_funcs[10])() = { &draw_generic_map, &draw_map1, &draw_map2, &draw_map3 };

static int (*actions_funcs[10])(int, int) = { &actions_generic_map, &actions_map1, &actions_map2, &actions_map3 };

static void (*grass_funcs[10])() = { &grass_generic_map, &grass_map1, &grass_map2, &grass_map3 };

void change_map_funcs(int map_num, void (**draw_map)(), int (**actions) (int, int), void (**grass_map)() ) {

    if (map_num < 1 || map_num > 3) {
        *draw_map = &draw_generic_map;
        *actions = &actions_generic_map;
        *grass_map = &grass_generic_map;
        return;
    }

    *draw_map = draw_funcs[map_num];
    *actions = actions_funcs[map_num];
    *grass_map = grass_funcs[map_num];
}


void back_to_poke_center() {
    begin_list();
    print_to_list("B scurried back to the Pokemon Center..."); sleep(4);
    heal_party();
    change_map(1, POKE_CENTER_X+4, POKE_CENTER_Y+3);
    sleep(1);
}