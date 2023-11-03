#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>

#include "print/print_utils.h"
#include "print/print_defines.h"
#include "load_save.h"
#include "player.h"
#include "motion/location.h"
#include "monsters/pokemon.h"
#include "motion/map_drawing.h"
#include "motion/motion2d.h"
#include "menu.h"



void test_function() {
    load_game(1);

    init_motion();
    change_map(player.loc->map, player.loc->x, player.loc->y);

    begin_message_box();
    print_to_message_box("Mr. and Mrs. Dursley, of number four, Privet Drive, were proud to say that they were perfectly normal, thank you very much. They were the last people you'd expect to be involved in anything strange or mysterious, because they just didn't hold with such nonsense.");
    
    await_user();
}