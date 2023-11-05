#ifndef KEY_ITEMS_H
#define KEY_ITEMS_H

#include "stdbool.h"

//All key item ID's
typedef enum { K_ITEM_EMPTY=200, K_ITEM_FOSSIL_HELIX, K_ITEM_FOSSIL_DOME, K_HELIX_VOUCHER, K_DOME_VOUCHER, SS_TICKET } Key_Item;

char * get_key_item_name(int id);

//Returns false if player already has the key item
bool add_key_item(Key_Item id);

//Returns false if player does not have the key item
bool remove_key_item(Key_Item id);

// Returns -1 if key_item not in list, else returns the index of the key item
int has_key_item(Key_Item id);

void handle_get_fossil();

void handle_process_fossil();

#endif // KEY_ITEMS_H