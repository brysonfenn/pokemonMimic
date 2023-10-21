#include "mart.h"

#include <ncurses.h>

#include "items.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"

static int last_selection = 0;

int * mart_array;

//Print all items available at the mart to the list box
void print_mart() {
  Item * currItem;
  char print_str[128] = "";

  mart_array = get_mart_array(2);

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
      case SELECT_CHAR:
        done_selecting = 1;
        break;
      case CANCEL_CHAR:
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

  if (inputNum == 0) {
    return ITEM_FAILURE;
  }

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