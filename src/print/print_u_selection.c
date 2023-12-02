#include "print_utils.h"

#include <ncurses.h>

#include "print_defines.h"

#define MAX_LIST 5

//Get player selection from scrollable list
//  str_list is the list of strings representing elements in list
//  num_in_list is the number of strings in the list
//  last_selection is where the cursor will begin (usually 0 or the last option selected)
int get_scrollable_selection(char * title, char ** str_list, int num_in_list, int last_selection) {
    char print_str[256];
    begin_list();

    if (num_in_list <= MAX_LIST) {
        print_to_list(title);
        for (int i = 0; i < num_in_list; i++) {
            sprintf(print_str, "  %s", str_list[i]);
            print_to_list(print_str);
        }
        return get_selection(1, num_in_list-1, last_selection);
    }

    int list_begin, list_end, cursor_y, cursor_x, ch, curr_list_index;
    
    //Set up last selection
    if (last_selection >= MAX_LIST) {
        cursor_y = LIST_BOX_Y + MAX_LIST + 1;
        list_begin = last_selection - MAX_LIST + 1;
        list_end = last_selection + 1;
    }
    else {
        cursor_y = LIST_BOX_Y + 2 + last_selection;
        list_begin = 0;
        list_end = MAX_LIST;
    }

    cursor_x = LIST_BOX_X + 1;
    curr_list_index = last_selection;
    ch = 0;

    while (1) {
        begin_list();
        // mvprintw(LIST_BOX_Y, LIST_BOX_X+LIST_BOX_WIDTH+2, "begin: %d", list_begin);
        // mvprintw(LIST_BOX_Y+1, LIST_BOX_X+LIST_BOX_WIDTH+2, "end: %d", list_end);
        // mvprintw(LIST_BOX_Y+2, LIST_BOX_X+LIST_BOX_WIDTH+2, "index: %d", curr_list_index);

        print_to_list(title);
        for (int i = list_begin; i < list_end; i++) {
            sprintf(print_str, "  %s", str_list[i]);
            print_to_list(print_str);
        }
        mvaddch(cursor_y, cursor_x, SELECTION_CHAR);

        flushinp();
        ch = getch();
        mvaddch(cursor_y, cursor_x, ' ');

        switch (ch) {
            case KEY_UP:
                if (curr_list_index == list_begin) {
                    if (list_begin == 0) {
                        cursor_y = LIST_BOX_Y + MAX_LIST + 1;
                        curr_list_index = num_in_list - 1;
                        list_begin = num_in_list - MAX_LIST;
                        list_end = num_in_list;
                    }
                    else {
                        curr_list_index--;
                        list_begin--;
                        list_end--;
                    }
                }
                else {
                    curr_list_index--;
                    cursor_y--;
                }
                break;
            case KEY_DOWN:
                if (curr_list_index == list_end - 1) {
                    if (curr_list_index == num_in_list - 1) {
                        cursor_y = LIST_BOX_Y + 2;
                        curr_list_index = 0;
                        list_begin = 0;
                        list_end = MAX_LIST;
                    }
                    else {
                        curr_list_index++;
                        list_begin++;
                        list_end++;
                    }
                }
                else {
                    curr_list_index++;
                    cursor_y++;
                }
                break;
            case SELECT_CHAR:
            case SELECT_CHAR_2:
                audio_play_file("select_button.mp3");
                return (curr_list_index);
            case CANCEL_CHAR:
            case CANCEL_CHAR_2:
                audio_play_file("back_button.mp3");
                return (PRESSED_B);
                break;
            default:
                break;
        }

        switch (ch) {
            case KEY_UP: case KEY_DOWN:
                audio_play_file("move_cursor.mp3");
                break;
            default: break; 
        }
    }


    return 0;
}