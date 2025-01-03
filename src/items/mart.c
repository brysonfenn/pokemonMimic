#include "mart.h"

#include <ncurses.h>

#include "items.h"
#include "../player.h"
#include "../motion/location.h"
#include "../motion/maps.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../audio/audio_player.h"

static int last_selection = 0;

static int * mart_array;
                               //NUM// ITEMS...
static int default_mart[10] =   { 1, FREEZE_HEAL };
static int viridian_mart[10] =  { 4, POKE_BALL, POTION, ANTIDOTE, PARALYZE_HEAL };
static int pewter_mart[10] =    { 6, POKE_BALL, POTION, ANTIDOTE, AWAKENING, PARALYZE_HEAL, REPEL };
static int cerulean_mart[10] =  { 7, POKE_BALL, POTION, SUPER_POTION, ANTIDOTE, PARALYZE_HEAL, BURN_HEAL, REPEL };
static int vermillion_mart[10] ={ 7, POKE_BALL, GREAT_BALL, POTION, SUPER_POTION, THUNDER_STONE, AWAKENING, SUPER_REPEL };
static int lavender_mart[10] =  { 6, POKE_BALL, GREAT_BALL, SUPER_POTION, MOON_STONE, ANTIDOTE, REPEL };
static int celadon_mart[10] =   { 8, POKE_BALL, GREAT_BALL, SUPER_POTION, LEAF_STONE, ANTIDOTE, REPEL, PARALYZE_HEAL, AWAKENING };
static int fuchsia_mart[10] =   { 7, GREAT_BALL, SUPER_POTION, WATER_STONE, ANTIDOTE, SUPER_REPEL, MAX_REPEL, PARALYZE_HEAL };
static int saffron_mart[10] =   { 8, ULTRA_BALL, GREAT_BALL, SUPER_POTION, HYPER_POTION, ANTIDOTE, MAX_REPEL, PARALYZE_HEAL, AWAKENING };
static int cinnabar_mart[10]=   { 8, ULTRA_BALL, SUPER_POTION, HYPER_POTION, SUPER_REPEL, MAX_REPEL, BURN_HEAL, AWAKENING, WATER_STONE };

void set_mart();

//Print all items available at the mart to the list box
void print_mart() {
    Item * currItem;
    char print_str[128] = "";

    for (int i = 1; i <= mart_array[0]; i++) {
        currItem = get_item_by_id(mart_array[i]);
        sprintf(print_str, "  %s",currItem->name);
        
        //handle spacing
        for (int j = strlen(currItem->name); j < 17; j++) sprintf(print_str, "%s ", print_str);

        sprintf(print_str, "%s$%d\n", print_str, currItem->cost);
        print_to_list(print_str);
    }
}


//Allow player to buy items
int handle_mart() {
    Item example_item;
    int inputNum, ch, maximum;
    char print_str[1024];

    set_mart();
    
    begin_list();
    print_to_list("Mart\n");
    print_mart();
    print_to_list("  Exit\n");
    sprintf(print_str, " \nYou have $%d\n", player.money);
    print_to_list(print_str);

    inputNum = get_selection(1, mart_array[0], last_selection);
    if (inputNum == PRESSED_B) inputNum = mart_array[0];
    last_selection = inputNum;

    inputNum++; //Align with ID number
    if (inputNum == mart_array[0]+1) { clear(); last_selection = 0; return ITEM_SUCCESS; }

    //   example_item = *(get_item_by_id(inputNum));
    example_item = *(get_item_by_id(mart_array[inputNum]));
    maximum = player.money / example_item.cost;

    begin_list();

    //Don't even let the player try if they can't buy something
    if (maximum == 0) {
        print_to_list("You do not have enough money to buy that!\n"); sleep(2);
        clear(); return ITEM_FAILURE;
    }
    else if (maximum > 99) {
        maximum = 99;
    }

    bool foundItem = false;
    int num_already_in_bag = 0;
    //Find if there were any of the selected item in the bag
    for (int i = 0; i < player.numInBag; i++) {
        if (player.bag[i].id_num == example_item.id_num) 
            num_already_in_bag = player.bag[i].number;
    }

    inputNum = 0;
    sprintf(print_str, "Select a quantity of %s(s) to buy: %02d\n \n", example_item.name, inputNum);
    sprintf(print_str, "%sCost: $%d\nYou have $%d\n \n \n", print_str, 0, player.money);
    sprintf(print_str, "%sYou currently have %d %s(s) in your bag\n", print_str, num_already_in_bag, example_item.name);
    print_to_list(print_str);

    int done_selecting = 0;
    
    while (1) {
        flushinp();
        ch = getch();
        switch (ch) {
            case KEY_UP: case KEY_DOWN:
                audio_play_file("move_cursor.mp3");
                break;
            default: break; 
        }

        switch (ch) {
            case KEY_UP:
                if (inputNum == maximum) inputNum = 0;
                else inputNum++;
                break;
            case KEY_DOWN:
                if (inputNum == 0) inputNum = maximum;
                else inputNum--;
                break;
            case SELECT_CHAR:
            case SELECT_CHAR_2:
                audio_play_file("select_button.mp3");
                done_selecting = 1;
                break;
            case CANCEL_CHAR:
            case CANCEL_CHAR_2:
                audio_play_file("back_button.mp3");
                return ITEM_FAILURE;
                break;
            default:
                break;
        }
        if (done_selecting) break;
        else { 
            begin_list();
            sprintf(print_str, "Select a quantity of %s(s) to buy: %02d\n \n", example_item.name, inputNum);
            sprintf(print_str, "%sCost: $%d\nYou have $%d\n \n \n", print_str, example_item.cost*inputNum, player.money);
            sprintf(print_str, "%sYou currently have %d %s(s) in your bag\n", print_str, num_already_in_bag, example_item.name);
            print_to_list(print_str);
        }
    }

    if (inputNum == 0) {
        return ITEM_FAILURE;
    }

    //Buy the item
    player.money -= inputNum * example_item.cost;

    give_item_to_player(&example_item, inputNum);

    sprintf(print_str, " \n \nYou bought %d %s(s)\n", inputNum, example_item.name);
    print_to_list(print_str); sleep(2); 

    clear();
    return ITEM_FAILURE;
}

void set_mart() {
    switch (player.loc->map) {
        case MAP_VIR_CITY:  mart_array =  viridian_mart; break;
        case MAP_PEW_CITY:  mart_array =  pewter_mart; break;
        case MAP_CER_CITY:  mart_array =  cerulean_mart; break;
        case MAP_VERM_CITY: mart_array =  vermillion_mart; break;
        case MAP_LAV_TOWN:  mart_array =  lavender_mart; break;
        case MAP_CEL_CITY:  mart_array =  celadon_mart; break;
        case MAP_FU_CITY:   mart_array =  fuchsia_mart; break;
        case MAP_SAFF_CITY: mart_array =  saffron_mart; break;
        case MAP_CIN_ISLAND:mart_array =  cinnabar_mart; break;
        default: mart_array = default_mart; break;
    }
}
