#include "items.h"

#define NUM_ITEMS 50
#define MAX_NUM_ITEM 4

                //  id  name           qt  cost    function        arg
item empty_item =   {0, "No Item     ", 1, 1000,  &do_nothing,      0};
item potion =       {1, "Potion      ", 0, 300,   &execute_potion,  5};
item super_potion = {2, "Super Potion", 0, 700,   &execute_potion,  10};
item pokeball =     {3, "Pokéball    ", 0, 200,   &attempt_catch,   60};
item greatball =    {4, "Great Ball  ", 0, 600,   &attempt_catch,   80};

//NOTE: Make sure to add item to this list after creating it
static * item_array[NUM_ITEMS] = { &empty_item, &potion, &super_potion, &pokeball, &greatball };

static pokemon * enemy_pok;

void items_init() {

}

void print_mart() {
  item * currItem;
  for (int i = 1; i <= MAX_NUM_ITEM; i++) {
    currItem = item_array[i];
    printf("%d: %s\t\t$%d\n\r", i, currItem->name, currItem->cost);
  }
}

int handle_mart() {
  item example_item;
  int inputNum;
  char example_string[50];

  printf("Mart\n\n");
  print_mart();
  printf("0: Exit\n");
  printf("\nYou have $%d\n", player.money);
  inputNum = getValidInput(0, MAX_NUM_ITEM, "Select item to buy: ");
  if (!inputNum) { return ITEM_SUCCESS; }
  else {
    example_item = *(get_item_by_id(inputNum));
    clearTerminal();
    sprintf(example_string, "Select a quantity of %s\nto buy: ", example_item.name);
    inputNum = getValidInput_force(0, 99, example_string, 3);
    if (inputNum * example_item.cost < player.money) {
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

      printf("You bought %d %s\n", inputNum, example_item.name); sleep(2);
    }
    else {
      printf("You do not have enough money to buy that!\n"); sleep(2);
      return ITEM_FAILURE;
    }
  }

  return ITEM_SUCCESS;
}

item * get_item_by_id(int id_num) {
  return item_array[id_num];
}

int use_item(int item_num, pokemon * enemy) {
  item * this_item = &(player.bag[item_num]);
  enemy_pok = enemy;

  int return_execute = this_item->execute(this_item->func_arg);
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

int execute_potion(int input_num) {
  clearTerminal();
  printParty();
  printf("0: Cancel\n");

  int input = getValidInput(0, player.numInParty, "\nSelect a pokemon to apply the potion on: ");
  if (!input) return ITEM_FAILURE;
  input--; //Adjust input number to array position

  int currentHP = player.party[input].currentHP;

  if (currentHP == 0 || currentHP == player.party[input].maxHP) {
    printf("Could not apply potion on %s.\n", player.party[input].name); sleep(1);
    return ITEM_FAILURE;
  }

  int hpGain = input_num;
  int tempHP = player.party[input].currentHP;

  player.party[input].currentHP += hpGain;
  if (player.party[input].currentHP > player.party[input].maxHP) {
    player.party[input].currentHP = player.party[input].maxHP;
  }

  printf("%s gained %d HP!\n", player.party[input].name, player.party[input].currentHP - tempHP); sleep(2);
  clearTerminal(); printParty(); sleep(2);

  return ITEM_SUCCESS;
}

int attempt_catch(int catch_rate) {
  if (enemy_pok->id_num == 0 || player.trainer_battle) {
    printf("You can't use that!\n"); sleep(2);
    return ITEM_FAILURE;
  }
  else if (player.numInParty >= 6) {
    printf("You already have 6 Pokemon!\n"); sleep(2);
    return ITEM_FAILURE;
  }

  catch_rate -= (enemy_pok->currentHP / 2);
  if (catch_rate < 1) catch_rate = 1;

  int random = rand() % 100;
  clearTerminal();
  printBattle();

  printf("B threw a Ball with catch_rate: %d\n", catch_rate); sleep(2);

  if (random < catch_rate) {
    printf("%s was caught!\n", enemy_pok->name); sleep(2);
    player.party[player.numInParty] = (*enemy_pok);
    player.numInParty++;
    return ITEM_CATCH_SUCCESS;
  }
  else {
    printf("Catch unsuccessful!\n"); sleep(2);
    return ITEM_CATCH_FAILURE;
  }
}

int do_nothing(int input_num) {
  return ITEM_FAILURE;
}