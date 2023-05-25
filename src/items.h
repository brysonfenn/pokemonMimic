#ifndef ITEMS_H
#define ITEMS_H

#define ITEM_SUCCESS 1
#define ITEM_FAILURE 0
#define ITEM_CATCH_SUCCESS 2
#define ITEM_CATCH_FAILURE 3
#define ITEM_END_BATTLE 4

#define NUM_ITEMS 50
#define MAX_NUM_ITEM 4

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

void items_init();

void print_mart();

int handle_mart();

Item * get_item_by_id(int id_num);

int use_item(int item_num, struct Pokemon * enemy);

int execute_potion(int input_num, char * name);

int attempt_catch(int catch_rate, char * name);

int do_nothing(int input_num);

#endif // ITEMS_H