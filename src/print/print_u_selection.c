#include "print_utils.h"

//Get player selection from scrollable list
//  str_list is the list of strings representing elements in list
//  num_in_list is the number of strings in the list
//  last_selection is where the cursor will begin (usually 0 or the last option selected)
int get_scrollable_selection(char ** str_list, int num_in_list, int last_selection) {
    char print_str[256];
    begin_list();

    for (int i = 0; i < num_in_list; i++) {
        sprintf(print_str, "  %s", str_list[i]);
        print_to_list(print_str);
    }

    return 0;
}