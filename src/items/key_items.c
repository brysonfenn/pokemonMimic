#include "key_items.h"

#include "../player.h"

static char key_item_names[10][32] = {"Empty Key Item", "Helix Fossil", "Dome Fossil"};


char * get_key_item_name(int id) {
    return key_item_names[id];
}

//Returns false if player already has the key item
bool add_key_item(Key_Item id) {
    if (has_key_item(id)) return false;
    player.key_items[player.numKeyItems] = id;
    player.numKeyItems++;
    return true;
}

bool has_key_item(Key_Item id) {
    for (int i = 0; i < player.numKeyItems; i++) {
        if (player.key_items[i] == id) {
            return true;
        }
    }

    return false;
}