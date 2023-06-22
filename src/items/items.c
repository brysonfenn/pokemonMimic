#include "items.h"

#include <ncurses.h>

#include "item_funcs.h"

#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../print/print_battle.h"
#include "../motion/location.h"
#include "../monsters/pokemon.h"
#include "../monsters/conditions.h"

#define CURRENT_MAX_NUM 8
#define NUM_ITEMS CURRENT_MAX_NUM + 1

                //  id  name           qt  cost    function        arg
Item empty_item   = {0, "No Item"     , 1, 1000,  &do_nothing,      0};
Item potion       = {1, "Potion"      , 0, 300,   &execute_potion,  20};
Item super_potion = {2, "Super Potion", 0, 700,   &execute_potion,  50};
Item pokeball     = {3, "Pokeball"    , 0, 200,   &attempt_catch,   100};
Item greatball    = {4, "Great Ball"  , 0, 600,   &attempt_catch,   150};
Item antidote     = {5, "Antidote"    , 0, 100,   &heal_condition,  POISONED};
Item paralyz_heal = {6, "Paralyz Heal", 0, 300,   &heal_condition,  PARALYZED};
Item awakening    = {7, "Awakening"   , 0, 250,   &heal_condition,  ASLEEP};
Item revive       = {8, "Revive"      , 0, 1500,  &revive_pokemon,  50};


//NOTE: Make sure to add item to this list after creating it
static * item_array[NUM_ITEMS] = { &empty_item, 
                    &potion, &super_potion, &pokeball, &greatball, &antidote, &paralyz_heal, &awakening, &revive };

static int last_selection = 0;

//Print all items available at the mart to the list box
void print_mart() {
  Item * currItem;
  char print_str[128];

  for (int i = 1; i <= CURRENT_MAX_NUM; i++) {
    currItem = item_array[i];
    sprintf(print_str, "  %s", currItem->name);
    
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
  
  begin_list();
  print_to_list("Mart\n");
  print_mart();
  print_to_list("  Exit\n");
  sprintf(print_str, " \nYou have $%d\n", player.money);
  print_to_list(print_str);

  inputNum = get_selection(1, CURRENT_MAX_NUM, last_selection);
  if (inputNum == PRESSED_B) inputNum = CURRENT_MAX_NUM;
  last_selection = inputNum;

  inputNum++; //Align with ID number
  if (inputNum == CURRENT_MAX_NUM+1) { clear(); return ITEM_SUCCESS; }

  example_item = *(get_item_by_id(inputNum));
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

  inputNum = 0;
  sprintf(print_str, "Select a quantity of %s(s) to buy: %02d\n \n", example_item.name, inputNum);
  sprintf(print_str, "%sCost: $%d\nYou have $%d\n", print_str, 0, player.money);
  print_to_list(print_str);

  int done_selecting = 0;
  
  while (1) {
    ch = getch();
    switch (ch) {
      case KEY_UP:
        if (inputNum == maximum) inputNum = 0;
        else inputNum++;
        break;
      case KEY_DOWN:
        if (inputNum == 0) inputNum = maximum;
        else inputNum--;
        break;
      case 'a':
        done_selecting = 1;
        break;
      case 'b':
        return ITEM_FAILURE;
        break;
      default:
        break;
    }
    if (done_selecting) break;
    else { 
      begin_list();
      sprintf(print_str, "Select a quantity of %s(s) to buy: %02d\n \n", example_item.name, inputNum);
      sprintf(print_str, "%sCost: $%d\nYou have $%d\n", print_str, example_item.cost*inputNum, player.money);
      print_to_list(print_str);
    }
  }

  if (inputNum == 0) return ITEM_FAILURE;

  //Buy the item
  player.money -= inputNum * example_item.cost;

  bool foundItem = false;
  //Find if there were any of those in the bag
  for (int i = 0; i < player.numInBag; i++) {
    if (player.bag[i].id_num == example_item.id_num) {
      foundItem = true;
      player.bag[i].number += inputNum;
    }
  }

  if (!foundItem) {
    player.bag[player.numInBag] = example_item;
    player.bag[player.numInBag].number = inputNum;
    player.numInBag++;
  }

  sprintf(print_str, " \n \nYou bought %d %s(s)\n", inputNum, example_item.name);
  print_to_list(print_str); sleep(2); 

  clear();
  return ITEM_FAILURE;
}


//Returns an item pointer by its ID number. Should be immediately dereferenced
Item * get_item_by_id(int id_num) {
  return item_array[id_num];
}


//Use an item, calling its execute function
int use_item(int item_num) {
  Item * this_item = &(player.bag[item_num]);

  int return_execute = this_item->execute(this_item->func_arg, this_item->name);

  if (return_execute == ITEM_SUCCESS || return_execute == ITEM_CATCH_SUCCESS || return_execute == ITEM_CATCH_FAILURE) {
    this_item->number--;
    //If number of items there is zero, adjust entire bag
    if (!this_item->number) {
      player.numInBag--;
      for (int i = item_num; i < player.numInBag; i++) {
        player.bag[i] = player.bag[i+1];
      }
      player.bag[player.numInBag] = empty_item;
    }
  }
  else if (return_execute == ITEM_FAILURE) {
    
  }

  return return_execute;
}

