#include "map_set7.h"

#include <ncurses.h>

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../selectables.h"
#include "../maps.h"

#include "../../player.h"
#include "../../battles/trainer.h"
#include "../../print/print_utils.h"
#include "../../print/print_defines.h"
#include "../../items/key_items.h"


void draw_safari1() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");

    add_npc_by_id(MAP_X+31, MAP_Y+13, 21, PLAYER_MOVING_DOWN);

    // add_portal_big_map_to_building(MAP_X+18, MAP_Y+1, MAP_FU_CITY, MAP_BOTTOM, 25);
    add_door(MAP_X+25, MAP_Y+MAP_HEIGHT-1, -3, true);
    draw_town_exit(MAP_BOTTOM, 25);

    add_connection_to_big_map(MAP_LEFT, 14, MAP_SAFARI2);
    add_connection_to_big_map(MAP_LEFT, 2, MAP_SAFARI2);
    add_connection_to_big_map(MAP_LEFT, 8, MAP_SAFARI2);

    add_connection_to_big_map(MAP_TOP, 6, MAP_SAFARI4);
    add_connection_to_big_map(MAP_TOP, 53, MAP_SAFARI4);

    add_surf_selectable(MAP_X+4, MAP_Y+5);
    add_surf_selectable(MAP_X+4, MAP_Y+10);
}

void draw_safari2() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");
    
    add_connection_to_big_map(MAP_RIGHT, 14, MAP_SAFARI1);
    add_connection_to_big_map(MAP_RIGHT, 2, MAP_SAFARI1);
    add_connection_to_big_map(MAP_RIGHT, 8, MAP_SAFARI1);

    add_connection_to_big_map(MAP_TOP, 32, MAP_SAFARI3);
    add_connection_to_big_map(MAP_TOP, 6, MAP_SAFARI3);

    add_surf_selectable(MAP_X+5, MAP_Y+5);
    add_surf_selectable(MAP_X+5, MAP_Y+10);
}

void draw_safari3() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 32, MAP_SAFARI2);
    add_connection_to_big_map(MAP_BOTTOM, 6, MAP_SAFARI2);
    add_connection_to_big_map(MAP_RIGHT, 15, MAP_SAFARI4);
    add_connection_to_big_map(MAP_RIGHT, 5, MAP_SAFARI4);
}

void draw_safari4() {
    draw_big_map("Safari Zone");
    audio_loop_file("evolution_theme.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 6, MAP_SAFARI1);
    add_connection_to_big_map(MAP_BOTTOM, 53, MAP_SAFARI1);
    add_connection_to_big_map(MAP_LEFT, 15, MAP_SAFARI3);
    add_connection_to_big_map(MAP_LEFT, 5, MAP_SAFARI3);

    add_npc_by_id(MAP_X+42, MAP_Y+10, 22, PLAYER_MOVING_DOWN);
}

void draw_route16() {
    draw_big_map("Route 16");
    audio_loop_file("beach.mp3");

    add_connection_to_big_map(MAP_TOP, 36, MAP_FU_CITY);
    add_connection_to_big_map(MAP_LEFT, 5, MAP_CIN_ISLAND);
}

void draw_saffron_city() {
    draw_big_map("Saffron City");
    audio_loop_file("vir_city.mp3");

    add_surf_selectable(MAP_X+29, MAP_Y+4);
    add_connection_to_big_map(MAP_TOP, 29, MAP_CER_CITY);
    add_surf_selectable(MAP_X+29, MAP_Y+13);
    add_connection_to_big_map(MAP_BOTTOM, 29, MAP_VERM_CITY);

    add_surf_selectable(MAP_X+54, MAP_Y+11);
    add_surf_selectable(MAP_X+54, MAP_Y+8);
    add_connection_to_big_map(MAP_RIGHT, 13, MAP_LAV_TOWN);

    add_surf_selectable(MAP_X+3, MAP_Y+13);
    add_surf_selectable(MAP_X+3, MAP_Y+10);
    add_connection_to_big_map(MAP_LEFT, 8, MAP_CEL_CITY);

    draw_building_default(MAP_X+11, MAP_Y+3, "POKE", POKE_CENTER_ACTION);
    draw_building_default(MAP_X+40, MAP_Y+10, "MART", MART_ACTION);

    draw_building_default(MAP_X+20, MAP_Y+8, "SLPH", -1);
    add_portal_building_to_big_map(MAP_X+20, MAP_Y+8, MAP_SILPH1, MAP_BOTTOM, 6);
    
    draw_building_default(MAP_X+38, MAP_Y+4, "Gym", -1);
    add_building_portal(MAP_X+38, MAP_Y+4, MAP_GYM6);

    if (! ((player.record_bits >> RECORD_BIT_15) & 0x01)) {
        add_npc_by_id(MAP_X+38+DEFAULT_BUILDING_WIDTH/2, MAP_Y+4+DEFAULT_BUILDING_HEIGHT-1, 25, PLAYER_MOVING_DOWN);
        add_npc_by_id(MAP_X+38+DEFAULT_BUILDING_WIDTH/2 + 1, MAP_Y+4+DEFAULT_BUILDING_HEIGHT-1, 25, PLAYER_MOVING_DOWN);
    }
}

void draw_gym6() {
    draw_interior(MAP_X+38, MAP_Y+4, MAP_SAFF_CITY);
    begin_message_box();
    print_to_message_box("Saffron City Gym");
    audio_loop_file("gym.mp3");

    add_trainer_by_id(INTERIOR_X+3, INTERIOR_Y+1, 96, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+12, INTERIOR_Y+1, 97, PLAYER_MOVING_DOWN);
    add_trainer_by_id(INTERIOR_X+23, INTERIOR_Y+4, 98, PLAYER_MOVING_DOWN);

    mvaddch(INTERIOR_Y+1, INTERIOR_X+24, ACS_LRCORNER);
    add_trainer_by_id(INTERIOR_X+25, INTERIOR_Y+1, 206, PLAYER_MOVING_DOWN);
    mvaddch(INTERIOR_Y+1, INTERIOR_X+26, ACS_LLCORNER);
}



//Silph co order:
// 4 5 6
// 1 2 3
void draw_silph1() {
    draw_big_map("Silph Co. Sector 1");
    audio_loop_file("silph_co.mp3");

    add_portal_big_map_to_building(MAP_X+20, MAP_Y+8, MAP_SAFF_CITY, MAP_BOTTOM, 6);

    add_connection_to_big_map(MAP_RIGHT, 3, MAP_SILPH2);
    add_connection_to_big_map(MAP_RIGHT, 11, MAP_SILPH2);

    add_connection_to_big_map(MAP_TOP, 3, MAP_SILPH4);
    add_connection_to_big_map(MAP_TOP, 23, MAP_SILPH4);
    add_connection_to_big_map(MAP_TOP, 54, MAP_SILPH4);

    add_locked_door(MAP_X+8, MAP_Y+11, K_ITEM_KEY_SILPH);
    add_locked_door(MAP_X+38, MAP_Y+9, K_ITEM_KEY_SILPH);

    
}

void draw_silph2() {
    draw_big_map("Silph Co. Sector 2");
    audio_loop_file("silph_co.mp3");

    add_connection_to_big_map(MAP_LEFT, 3, MAP_SILPH1);
    add_connection_to_big_map(MAP_LEFT, 11, MAP_SILPH1);
    add_connection_to_big_map(MAP_RIGHT, 4, MAP_SILPH3);
    add_connection_to_big_map(MAP_RIGHT, 9, MAP_SILPH3);
    add_connection_to_big_map(MAP_RIGHT, 14, MAP_SILPH3);

    add_connection_to_big_map(MAP_TOP, 4, MAP_SILPH5);
    add_connection_to_big_map(MAP_TOP, 15, MAP_SILPH5);
    add_connection_to_big_map(MAP_TOP, 30, MAP_SILPH5);

    add_locked_door(MAP_X+18, MAP_Y+7, K_ITEM_KEY_SILPH);
    add_locked_door(MAP_X+47, MAP_Y+7, K_ITEM_KEY_SILPH);

}

void draw_silph3() {
    draw_big_map("Silph Co. Sector 3");
    audio_loop_file("silph_co.mp3");

    add_connection_to_big_map(MAP_LEFT, 4, MAP_SILPH2);
    add_connection_to_big_map(MAP_LEFT, 9, MAP_SILPH2);
    add_connection_to_big_map(MAP_LEFT, 14, MAP_SILPH2);

    add_connection_to_big_map(MAP_TOP, 53, MAP_SILPH6);
    add_connection_to_big_map(MAP_TOP, 43, MAP_SILPH6);

}

void draw_silph4() {
    draw_big_map("Silph Co. Sector 4");
    audio_loop_file("silph_co.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 3, MAP_SILPH1);
    add_connection_to_big_map(MAP_BOTTOM, 23, MAP_SILPH1);
    add_connection_to_big_map(MAP_BOTTOM, 54, MAP_SILPH1);

    add_connection_to_big_map(MAP_RIGHT, 4, MAP_SILPH5);

    add_locked_door(MAP_X+4, MAP_Y+7, K_ITEM_KEY_SILPH);
    add_locked_door(MAP_X+23, MAP_Y+7, K_ITEM_KEY_SILPH);
    add_locked_door(MAP_X+47, MAP_Y+5, K_ITEM_KEY_SILPH);

}

void draw_silph5() {
    draw_big_map("Silph Co. Sector 5");
    audio_loop_file("silph_co.mp3");

    add_connection_to_big_map(MAP_RIGHT, 3, MAP_SILPH6);
    add_connection_to_big_map(MAP_RIGHT, 12, MAP_SILPH6);

    add_connection_to_big_map(MAP_BOTTOM, 4, MAP_SILPH2);
    add_connection_to_big_map(MAP_BOTTOM, 15, MAP_SILPH2);
    add_connection_to_big_map(MAP_BOTTOM, 30, MAP_SILPH2);

    add_connection_to_big_map(MAP_LEFT, 4, MAP_SILPH4);

    add_locked_door(MAP_X+39, MAP_Y+11, K_ITEM_KEY_SILPH);
    
}

void draw_silph6() {
    draw_big_map("Silph Co. Sector 6");
    audio_loop_file("silph_co.mp3");

    add_connection_to_big_map(MAP_BOTTOM, 53, MAP_SILPH3);
    add_connection_to_big_map(MAP_BOTTOM, 43, MAP_SILPH3);
    add_connection_to_big_map(MAP_LEFT, 3, MAP_SILPH5);
    add_connection_to_big_map(MAP_LEFT, 12, MAP_SILPH5);

    add_locked_door(MAP_X+47, MAP_Y+7, K_ITEM_KEY_SILPH);
}