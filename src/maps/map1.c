#include "map1.h"

#include "ncurses.h"
#include "map_drawing.h"
#include "../player.h"
#include "location.h"
#include "doors.h"
#include "../battles/trainer.h"
#include "../print_utils.h"

#define MART_ACTION 1
#define POKE_CENTER_ACTION 2
#define TRAINER_BATTLE_ACTION 3

#define GRASS_CHAR 'M'


void draw_map1() {

    mvprintw(22, 1, "Press 'm' to return to the menu");
    drawBuilding_default(MART_X,MART_Y, "Mart", MART_ACTION);
    drawBuilding_default(POKE_CENTER_X,POKE_CENTER_Y, "Poke", POKE_CENTER_ACTION);
    drawBox(MAP_X,MAP_Y,TOWN_WIDTH,TOWN_HEIGHT);  //Draw Town border

    //Draw Doors
    draw_town_exit(MAP_TOP, 10);
    draw_town_exit(MAP_BOTTOM, 10);
    draw_town_exit(MAP_LEFT, 10);
    add_door(MAP_X, MAP_Y+10, TRAINER_BATTLE_ACTION);
    draw_town_exit(MAP_RIGHT, 10);

    grass_map1();
}

int actions_map1(int player_x, int player_y) {
    int action = get_door_action(player_x, player_y);
    if (!action) return 0;

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
    mvaddch(MAP_Y+10, MAP_X+40, GRASS_CHAR); 
    mvaddch(MAP_Y+10, MAP_X+41, GRASS_CHAR); 
    mvaddch(MAP_Y+10, MAP_X+42, GRASS_CHAR); 
}