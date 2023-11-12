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



static char test_str[64] = "";
static char test_str2[64] = "";


void print_alphabet(char curr_char);

void test_function() {
    sprintf(test_str, "%s", get_name_input("yourself"));

    begin_list();
    sprintf(test_str2, "Entered String: %s", test_str);
    print_to_list(test_str2); sleep(2);
}