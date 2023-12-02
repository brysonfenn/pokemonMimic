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
    char name4[50] = "D";
    char name5[50] = "E";
    char name6[50] = "F";
    char name7[50] = "G";
    char name8[50] = "H";
    char name9[50] = "I";

    char * list[10];
    list[0] = name1;
    list[1] = name2;
    list[2] = name3;
    list[3] = name4;
    list[4] = name5;
    list[5] = name6;
    list[6] = name7;
    list[7] = name8;
    list[8] = name9;

    int sel = get_scrollable_selection("Pokemon:", list, 9, 7);

    sprintf(test_str, " \nGot Selection: %s", list[sel]);
    print_to_list(test_str);

    print_to_list(" \nPress Enter to quit..."); refresh();
    sleep(2);
    getch();
}