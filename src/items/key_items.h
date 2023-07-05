#ifndef KEY_ITEMS_H
#define KEY_ITEMS_H

#include "stdbool.h"

//All key item ID's
typedef enum { K_ITEM_EMPTY, K_ITEM_FOSSIL_HELIX, K_ITEM_FOSSIL_DOME } Key_Item;

char * get_key_item_name(int id);

//Returns false if player already has the key item
bool add_key_item(Key_Item id);

bool has_key_item(Key_Item id);

#endif // KEY_ITEMS_H