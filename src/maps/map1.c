#include "map1.h"

#include "ncurses.h"
#include "map_drawing.h"
#include "../player.h"
#include "location.h"
#include "doors.h"
#include "../battles/trainer.h"
#include "../print_utils.h"
#include "../items.h"
#include "../print_defines.h"

#include "motion2d.h"

#define MART_ACTION 1
#define POKE_CENTER_ACTION 2
#define TRAINER_BATTLE_ACTION 3

void draw_map1() {

    mvprintw(22, 1, "Press 'm' to return to the menu");
    drawBox(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y);

    drawBuilding_default(MART_X,MART_Y, "Mart", MART_ACTION);
    drawBuilding_default(POKE_CENTER_X,POKE_CENTER_Y, "Poke", POKE_CENTER_ACTION);
    
    //Draw Doors
    draw_town_exit(MAP_TOP, 10); 
    add_exit_portal(MAP_TOP, 10, 2);
    
    draw_town_exit(MAP_BOTTOM, 25); 
    add_exit_portal(MAP_BOTTOM, 25, 3);

    draw_town_exit(MAP_LEFT, 10);
    add_door(MAP_X, MAP_Y+10, TRAINER_BATTLE_ACTION, 0);

    draw_town_exit(MAP_RIGHT, 10);
    add_exit_portal(MAP_RIGHT, 10, 4);

    grass_map1();

    refresh();
}

int actions_map1(int player_x, int player_y) {
    Location door = *(get_door(player_x, player_y));
    int action = door.action;
    if (!action) return 0;

    //Handle portal
    if (action == -1) {
        change_map(door.next_map, door.next_x, door.next_y);
        return -1;
    }

    sleep(1);

    clear();

    switch (action) {
        case MART_ACTION:
            while (handle_mart() == ITEM_FAILURE) { clear(); }
            player.loc->y += 1;
            break;
        case POKE_CENTER_ACTION:
            handle_poke_center();
            player.loc->y += 1;
            break;
        case TRAINER_BATTLE_ACTION:
            battle_trainer();
            player.loc->x += 1;
            break;
        default:
            break;
    }

    clear();

    return action;
}

void grass_map1() {

    attrset(COLOR_PAIR(GRASS_COLOR));
    mvaddch(MAP_Y+10, MAP_X+40, GRASS_CHAR); 
    mvaddch(MAP_Y+10, MAP_X+41, GRASS_CHAR); 
    mvaddch(MAP_Y+10, MAP_X+42, GRASS_CHAR); 
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}