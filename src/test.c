#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>

#include "player.h"
#include "print/print_utils.h"
#include "print/print_defines.h"
#include "audio/audio_player.h"


static char test_str[64] = "";
static char test_str2[64] = "";


void test_function() {

    char name1[50] = "Bryson";
    char name2[50] = "Olivia";
    char name3[50] = "Allison";

    char * list[5];
    list[0] = name1;
    list[1] = name2;
    list[2] = name3;

    get_scrollable_selection(list, 3, 0);

    printw("Press Enter to quit..."); refresh();
    sleep(2);
    getch();
}