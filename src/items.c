#include "items.h"

#include <ncurses.h>

#include "print_utils.h"
#include "print_defines.h"
#include "player.h"
#include "monsters/pokemon.h"

#define NUM_ITEMS 50
#define CURRENT_MAX_NUM 4

                //  id  name           qt  cost    function        arg
item empty_item =   {0, "No Item"     , 1, 1000,  &do_nothing,      0};
item potion =       {1, "Potion"      , 0, 300,   &execute_potion,  20};
item super_potion = {2, "Super Potion", 0, 700,   &execute_potion,  50};
item pokeball =     {3, "Pokeball"    , 0, 200,   &attempt_catch,   60};
item greatball =    {4, "Great Ball"  , 0, 600,   &attempt_catch,   80};

//NOTE: Make sure to add item to this list after creating it
static * item_array[NUM_ITEMS] = { &empty_item, &potion, &super_potion, &pokeball, &greatball };

static pokemon * enemy_pok;
static int last_selection = 0;

void items_init() {

}

void print_mart() {
  item * currItem;
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

int handle_mart() {
  item example_item;
  int inputNum, ch, maximum;
  char print_str[1024];
  
  begin_list();
  print_to_list("Mart\n");
  print_mart();
  print_to_list("  Exit\n");
  sprintf(print_str, " \nYou have $%d\n", player.money);
  print_to_list(print_str);

  inputNum = get_selection(LIST_BOX_Y+2, 0, CURRENT_MAX_NUM,last_selection,NOT_MAIN_SELECT);
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

item * get_item_by_id(int id_num) {
  return item_array[id_num];
}

int use_item(int item_num, struct pokemon * enemy) {
  item * this_item = &(player.bag[item_num]);
  enemy_pok = enemy;

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

int execute_potion(int input_num, char * name) {
  char print_str[4096];

  begin_list();
  sprintf(print_str, "Select a pokemon to apply the %s on.\n", name);
  print_to_list(print_str);
  printParty();
  print_to_list("  Cancel\n");

  int input = get_selection(LIST_BOX_Y+3,0,player.numInParty,0,NOT_MAIN_SELECT);
  if (input == player.numInParty) return ITEM_FAILURE;

  int currentHP = player.party[input].currentHP;

  if (currentHP == 0 || currentHP == player.party[input].maxHP) {
    sprintf(print_str, " \nCould not apply %s on %s.\n", name, player.party[input].name);
    print_to_list(print_str); sleep(2);
    return ITEM_FAILURE;
  }

  int hpGain = input_num;
  int tempHP = player.party[input].currentHP;

  player.party[input].currentHP += hpGain;
  if (player.party[input].currentHP > player.party[input].maxHP) {
    player.party[input].currentHP = player.party[input].maxHP;
  }

  sprintf(print_str, " \n%s gained %d HP!\n", player.party[input].name, player.party[input].currentHP - tempHP);
  print_to_list(print_str); sleep(2);

  begin_list();
  printParty(); refresh(); sleep(2);

  return ITEM_SUCCESS;
}

int attempt_catch(int catch_rate, char * name) {
  if (enemy_pok->id_num == 0 || player.trainer_battle) {
    begin_list();
    print_to_list("You can't use that!"); sleep(2);
    return ITEM_FAILURE;
  }
  else if (player.numInParty >= 6) {
    begin_list();
    print_to_list("You already have 6 Pokemon!"); sleep(2);
    return ITEM_FAILURE;
  }

  catch_rate -= (enemy_pok->currentHP / 2);
  if (catch_rate < 1) catch_rate = 1;

  int random = rand() % 100;
  clear();
  printBattle();

  text_box_cursors(TEXT_BOX_BEGINNING);
  printw("B threw a %s!", name); refresh(); sleep(2);

  text_box_cursors(TEXT_BOX_NEXT_LINE);
  if (random < catch_rate) {
    printw("%s was caught!", enemy_pok->name); refresh(); sleep(2);
    player.party[player.numInParty] = (*enemy_pok);
    player.numInParty++;
    return ITEM_CATCH_SUCCESS;
  }
  else {
    printw("Catch unsuccessful!"); refresh(); sleep(2);
    return ITEM_CATCH_FAILURE;
  }
}

int do_nothing(int input_num) {
  return ITEM_FAILURE;
}