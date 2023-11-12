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


#define BEGIN_X MAP_X + 2
#define BEGIN_Y MAP_Y + 2
char example_str[30] = "";


void print_alphabet(char curr_char);

void test_function() {
    char curr_char = ' ';
    int ch, string_length = 0;
    print_alphabet(curr_char);
    sprintf(example_str, "");
    mvprintw(MAP_Y+17, MAP_X+3, "[%s]", example_str);

    while (1) {
        ch = getch();

        switch (ch) {
            case KEY_UP:
                if (curr_char - 14 >= ' ') curr_char -= 14;
                break;
            case KEY_DOWN:
                if (curr_char + 14 <= 'z') curr_char += 14;
                break;
            case KEY_LEFT:
                if (curr_char == 't') curr_char = 'z';
                else if ((curr_char - ' ') % 14 == 0) curr_char += 13;
                else curr_char--;
                break;
            case KEY_RIGHT:
                if (curr_char == 'z') curr_char = 't';
                else if ((curr_char - ' ') % 14 == 13) curr_char -= 13;
                else curr_char++;
                break;
            case SELECT_CHAR:
                string_length = strlen(example_str);
                if (string_length < 30) {
                    example_str[string_length] = curr_char;
                    example_str[string_length+1] = '\0';
                }
                break;
            case CANCEL_CHAR:
                string_length = strlen(example_str);
                if (string_length > 0) {
                    example_str[string_length-1] = '\0';
                }
                break;
            default:
                break;
        }

        print_alphabet(curr_char);
        mvprintw(MAP_Y+17, MAP_X+3, "[%s]", example_str);
        

    }

    await_user();
}


void print_alphabet(char curr_char) {
    int curr_x = BEGIN_X;
    int curr_y = BEGIN_Y;
    char c = ' ';

    begin_list();

    while (c <= 'z') {
        if (c == curr_char) attrset(COLOR_PAIR(INVERSE_COLOR));
        mvprintw(curr_y, curr_x, "%c", c);
        curr_x += 4;
        if (curr_x >= MAP_X+MAP_WIDTH) {
            curr_x = BEGIN_X;
            curr_y += 2;
        }
        if (c == curr_char) attrset(COLOR_PAIR(DEFAULT_COLOR));
        c++;
    }
}