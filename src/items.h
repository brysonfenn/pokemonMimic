#ifndef ITEMS_H
#define ITEMS_H

#include "stdio.h"
#include "print_utils.h"
#include "player.h"
#include "pokemon.h"

#define ITEM_SUCCESS 1
#define ITEM_FAILURE 0
#define ITEM_CATCH_SUCCESS 2
#define ITEM_CATCH_FAILURE 3
#define ITEM_END_BATTLE 4

#define NUM_ITEMS 50
#define MAX_NUM_ITEM 4

typedef int (*item_func_ptr) (int);

typedef struct item {
  int id_num;
  char name[30];
  int number;
  int cost;
  item_func_ptr execute;
  int func_arg;
} item;

extern item empty_item;
extern item potion;
extern item super_potion;
extern item pokeball;
extern item greatball;

void items_init();

void print_mart();

int handle_mart();

item * get_item_by_id(int id_num);

int use_item(int item_num, pokemon * enemy);

int execute_potion(int input_num);

int attempt_catch(int catch_rate);

int do_nothing(int input_num);

#endif // ITEMS_H