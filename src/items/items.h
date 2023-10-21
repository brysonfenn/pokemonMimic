#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>

#define ITEM_SUCCESS 1
#define ITEM_FAILURE 0
#define ITEM_CATCH_SUCCESS 2
#define ITEM_CATCH_FAILURE 3
#define ITEM_END_BATTLE 4

#define NUM_ITEMS 50
#define MAX_NUM_ITEM 4

#define CURRENT_MAX_NUM 8
#define NUM_ITEMS CURRENT_MAX_NUM + 1

struct Pokemon;

typedef int (*item_func_ptr) (int, char *);

typedef struct Item {
  int id_num;
  char name[30];
  int number;
  int cost;
  item_func_ptr execute;
  int func_arg;
} Item;

extern Item empty_item;
extern Item potion;
extern Item super_potion;
extern Item pokeball;
extern Item greatball;

//Returns an item pointer by its ID number. Should be immediately dereferenced
Item * get_item_by_id(int id_num);

//Returns a mart array based on an ID number identifying the city where the mart is located
int * get_mart_array(int id_num);

//Use an item, calling its execute function
//  decrement amount if return value is not ITEM_FAILURE
int use_item(int item_num);

#endif // ITEMS_H