#include "map2.h"

#include "ncurses.h"

#include "../map_drawing.h"
#include "../location.h"
#include "../doors.h"
#include "../selectables.h"

#include "../../player.h"
#include "../../battles/trainer.h"
#include "../../print_utils.h"
#include "../../print_defines.h"

#define MART_ACTION 1
#define POKE_CENTER_ACTION 2
#define TRAINER_BATTLE_ACTION 3

static Trainer trainer3 = {"Trainer John", 3, {POKEMON_IVYSAUR, POKEMON_CHARMELEON, POKEMON_WARTORTLE}, {4,5,6} };
static Trainer trainer4 = {"Bugcatcher David", 1, {POKEMON_KAKUNA}, {7} };

void draw_map2() {

    mvprintw(22, 1, "Press 'm' to return to the menu");
    drawBox(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y);

    //Draw Doors
    draw_town_exit(MAP_TOP, 10);

    draw_town_exit(MAP_BOTTOM, 10);
    add_exit_portal(MAP_BOTTOM, 10, 1);
    
    add_trainer(MAP_X+1, MAP_Y+8, &trainer3, PLAYER_MOVING_RIGHT);
    add_trainer(MAP_X+15, MAP_Y+5, &trainer4, PLAYER_MOVING_LEFT);

    grass_map2();

    refresh();
}

int actions_map2(int player_x, int player_y) {
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
        default:
            break;
    }

    clear();

    return action;
}

void grass_map2() {

    attrset(COLOR_PAIR(GRASS_COLOR));
    for (int i = 5; i < 30; i++) {
        mvaddch(MAP_Y+13, MAP_X+i, GRASS_CHAR); 
        mvaddch(MAP_Y+14, MAP_X+i, GRASS_CHAR); 
        mvaddch(MAP_Y+15, MAP_X+i, GRASS_CHAR); 
    }
    attrset(COLOR_PAIR(DEFAULT_COLOR));
}