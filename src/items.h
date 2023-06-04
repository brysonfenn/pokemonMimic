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

//Print all items available at the mart to the list box
void print_mart();

//Allow player to buy items
int handle_mart();

//Returns an item pointer by its ID number. Should be immediately dereferenced
Item * get_item_by_id(int id_num);

//Use an item, calling its execute function
//  decrement amount if return value is not ITEM_FAILURE
int use_item(int item_num, struct Pokemon * enemy);

//Potion execute function
int execute_potion(int hp_gain, char * name);

//Pokeball execute function (catch_rate is % multiplier)
int attempt_catch(int catch_rate, char * name);

//Filler function for empty_item that does nothing
int do_nothing(int input_num);

#endif // ITEMS_H