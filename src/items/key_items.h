#ifndef KEY_ITEMS_H
#define KEY_ITEMS_H

#include "stdbool.h"

//All key item ID's
typedef enum { K_ITEM_EMPTY=200, K_ITEM_FOSSIL_HELIX, K_ITEM_FOSSIL_DOME, K_ITEM_FOSSIL_AMBER,
                K_HELIX_VOUCHER, K_DOME_VOUCHER, K_AMBER_VOUCHER, K_ITEM_SS_TICKET, K_ITEM_FLUTE,
                K_ITEM_SILPH_SCOPE, K_ITEM_SURF_FLAG, K_ITEM_KEY_SILPH, K_ITEM_KEY_CINNABAR } Key_Item;

char * get_key_item_name(int id);

//Returns false if player already has the key item
bool add_key_item(Key_Item id);

//Returns false if player does not have the key item
bool remove_key_item(Key_Item id);

void handle_get_fossil();

void handle_process_fossil();

#endif // KEY_ITEMS_H