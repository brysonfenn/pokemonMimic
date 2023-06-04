#include "map2.h"

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


static Trainer trainer1 = {1, "Trainer John", 2, {POKEMON_RATTATA, POKEMON_PIDGEY}, {4,4} };
static Trainer trainer2 = {2, "Bugcatcher David", 2, {POKEMON_KAKUNA, POKEMON_WEEDLE}, {4,5} };
static Trainer trainer3 = {3, "Trainer Steven", 2, {POKEMON_SANDSHREW, POKEMON_BEEDRILL}, {4,5} };

void draw_map2() {

    draw_box(MAP_X,MAP_Y,MAP_WIDTH,MAP_HEIGHT);  //Draw Town border
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    begin_message_box();
    print_to_message_box("Route 2");

    //Draw Doors
    draw_town_exit(MAP_BOTTOM, 10);
    add_exit_portal(MAP_BOTTOM, 10, MAP_VIRIDIAN);

    draw_town_exit(MAP_TOP, 7);
    add_exit_portal(MAP_TOP, 7, MAP_VIR_FOREST);

    
    add_trainer(MAP_X+1, MAP_Y+8, &trainer1, PLAYER_MOVING_RIGHT);
    add_trainer(MAP_X+15, MAP_Y+6, &trainer2, PLAYER_MOVING_LEFT);
    add_trainer(MAP_X+1, MAP_Y+4, &trainer3, PLAYER_MOVING_RIGHT);

    refresh();
}