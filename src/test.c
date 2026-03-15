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
#include "monsters/pokemon.h"


static char test_str[64] = "";
static char test_str2[64] = "";


void test_function() {

    print_to_list(" \nPress Enter to continue..."); refresh();
    getch();

    Pokemon curr_pok; 
    curr_pok = * create_new_pokemon(30, 5, 5, 5);
    Pokemon * pok = &curr_pok;

    int base_exp = get_pokemon_base_exp(pok);

    begin_list();

    //Print result
    sprintf(test_str, " base_exp:%d", base_exp);
    print_to_list(test_str);

    print_to_list(" \nPress Enter to quit..."); refresh();
    getch();
}