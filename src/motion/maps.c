#include "maps.h"

#include <ncurses.h>

#include "maps/generic_map.h"
#include "maps/map1.h"
#include "maps/map2.h"
#include "maps/map3.h"
#include "maps/map4.h"

#include "motion2d.h"
#include "location.h"
#include "../player.h"
#include "../print_utils.h"

static void (*draw_funcs[10])() = { &draw_generic_map, &draw_map1, &draw_map2, &draw_map3, &draw_map4 };

static int (*actions_funcs[10])(int, int) = { &actions_generic_map, &actions_map1, &actions_map2, &actions_map3, &actions_map4 };

static void (*grass_funcs[10])() = { &grass_generic_map, &grass_map1, &grass_map2, &grass_map3, &draw_map4 };

void change_map_funcs(int map_num, void (**draw_map)(), int (**actions) (int, int), void (**grass_map)() ) {

    if (map_num < 1 || map_num > 4) {
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
    print_to_list("B scurried back to the Pokemon Center..."); sleep(2);
    heal_party();
    // change_map(player.blackout_center->map, POKE_CENTER_X+4, POKE_CENTER_Y+3);
    change_map(player.blackout_center->map, player.blackout_center->x, player.blackout_center->y);

    sleep(1);
}