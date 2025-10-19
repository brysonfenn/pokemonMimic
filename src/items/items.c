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

                //     id            name            qt  cost    function        arg
Item empty_item    = {NO_ITEM,       "No Item"      , 1, 1000,  &do_nothing,      0};                // 00

Item potion        = {POTION,        "Potion"       , 0, 300,   &execute_potion,  20};
Item super_potion  = {SUPER_POTION,  "Super Potion" , 0, 700,   &execute_potion,  50};
Item pokeball      = {POKE_BALL,     "Poke Ball"    , 0, 200,   &attempt_catch,   100};
Item greatball     = {GREAT_BALL,    "Great Ball"   , 0, 600,   &attempt_catch,   150};
Item ultraball     = {ULTRA_BALL,    "Ultra Ball"   , 0, 1200,  &attempt_catch,   200};
Item antidote      = {ANTIDOTE,      "Antidote"     , 0, 100,   &heal_condition,  POISONED};
Item paralyze_heal = {PARALYZE_HEAL, "Paralyze Heal", 0, 200,   &heal_condition,  PARALYZED};
Item awakening     = {AWAKENING,     "Awakening"    , 0, 250,   &heal_condition,  ASLEEP};
Item burn_heal     = {BURN_HEAL,     "Burn Heal"    , 0, 200,   &heal_condition,  BURNED};
Item freeze_heal   = {FREEZE_HEAL,   "Freeze Heal"  , 0, 200,   &heal_condition,  FROZEN};           // 10

Item revive        = {REVIVE,        "Revive"       , 0, 1500,  &revive_pokemon,    50};
Item fire_stone    = {FIRE_STONE,    "Fire Stone"   , 0, 5000,  &use_evolve_stone,  202};
Item water_stone   = {WATER_STONE,   "Water Stone"  , 0, 5000,  &use_evolve_stone,  203};
Item thunder_stone = {THUNDER_STONE, "Thunder Stone", 0, 5000,  &use_evolve_stone,  204};
Item leaf_stone    = {LEAF_STONE,    "Leaf Stone"   , 0, 5000,  &use_evolve_stone,  205};
Item moon_stone    = {MOON_STONE,    "Moon Stone"   , 0, 5000,  &use_evolve_stone,  206};
Item repel         = {REPEL,         "Repel"        , 0,  350,  &use_repel,         100};
Item super_repel   = {SUPER_REPEL,   "Super Repel"  , 0,  500,  &use_repel,         200};
Item max_repel     = {MAX_REPEL,     "Max Repel"    , 0,  700,  &use_repel,         250};
Item hyper_repel   = {HYPER_POTION,  "Hyper Potion" , 0, 1500,  &execute_potion,    200};
Item masterball    = {MASTER_BALL,   "Master Ball"  , 0,99999,  &attempt_catch,    2000};


//NOTE: Make sure to add item to this list after creating it
static * item_array[NUM_ITEMS] = { &empty_item, 
    &potion, &super_potion, &pokeball, &greatball, &ultraball, &antidote, &paralyze_heal, &awakening, &burn_heal, &freeze_heal,   //  1-10
    &revive, &fire_stone, &water_stone, &thunder_stone, &leaf_stone, &moon_stone, &repel, &super_repel, &max_repel, &hyper_repel, //  11-20 
    &masterball   
};

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


//Give player a number of items by id number
void give_item_to_player(Item * item_to_get, int num_items) {
    bool foundItem = false;
    //Find if there were any of the selected item in the bag
    for (int i = 0; i < player.numInBag; i++) {
        if (player.bag[i].id_num == (*item_to_get).id_num) {
            foundItem = true;
            player.bag[i].number += num_items;
        }
    }

    if (!foundItem) {
        player.bag[player.numInBag] = *(item_to_get);
        player.bag[player.numInBag].number = num_items;
        player.numInBag++;
    }
}
