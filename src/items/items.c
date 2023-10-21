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

static int viridian_mart[10] = {3, 1, 3, 5 };
static int pewter_mart[10] = {4, 1, 2, 3, 6 };
static int cerulean_mart[10] = {5, 2, 3, 4, 7, 8 };

static int * mart_arrays[10] = {
  viridian_mart, pewter_mart, cerulean_mart
};

//Returns an item pointer by its ID number. Should be immediately dereferenced
Item * get_item_by_id(int id_num) {
  return item_array[id_num];
}


//Returns a mart array based on an ID number identifying the city where the mart is located
int * get_mart_array(int id_num) {
  return mart_arrays[id_num];
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

