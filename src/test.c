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
#include "player.h"

#include "audio/audio_player.h"


static char test_str[64] = "";
static char test_str2[64] = "";


void test_function() {
    // play_audio_file("pallet.wav");

    loop_audio_file("pallet.wav");

    printw("Press Enter to quit..."); refresh();
    sleep(2);
    loop_audio_file("wild_battle.wav");
    getch();
}