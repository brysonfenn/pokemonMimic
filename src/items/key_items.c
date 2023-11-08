#include "key_items.h"

#include "../player.h"
#include "../motion/location.h"
#include "../print/print_utils.h"
#include "../monsters/pokemon.h"
#include "../monsters/conditions.h"

static char key_item_names[10][32] = {"Empty Key Item", "Helix Fossil", "Dome Fossil", "Helix Fossil Voucher",
    "Dome Fossil Voucher", "SS Ticket" };


void take_fossil(int fossil_index, int fossil_type);
void give_fossil_pokemon(int fossil_index, int fossil_type);

char * get_key_item_name(int id) {
    id -= K_ITEM_EMPTY;
    return key_item_names[id];
}

//Returns false if player already has the key item
bool add_key_item(Key_Item id) {
    if (has_key_item(id) >= 0) return false;
    player.key_items[player.numKeyItems] = id;
    player.numKeyItems++;
    return true;
}

//Returns false if player does not have the key item
bool remove_key_item(Key_Item id) {
    int item_index = has_key_item(id);
    if (item_index < 0) return false;

    player.numKeyItems--;
    for (int i = item_index; i < player.numKeyItems; i++) {
        player.key_items[i] = player.key_items[i+1];
    }
    return true;
}

// Returns -1 if key_item not in list, else returns the index of the key item
int has_key_item(Key_Item id) {
    for (int i = 0; i < player.numKeyItems; i++) {
        if (player.key_items[i] == id) {
            return i;
        }
    }
    return -1;
}

void handle_get_fossil() {
    clear();
    begin_list();

    if (has_key_item(K_ITEM_FOSSIL_HELIX) >= 0 || has_key_item(K_ITEM_FOSSIL_DOME) >= 0) {
        print_to_list("You already have a fossil!"); sleep(2);
        player.loc->y += 1;
        return;
    }

    print_to_list("Select one:\n  Helix Fossil\n  Dome Fossil\n  Cancel");
    int input = get_selection(1, 2, 0);
    if (input == 0) {
        player.key_items[player.numKeyItems] = K_ITEM_FOSSIL_HELIX;
        player.numKeyItems++;
        print_to_list("Received the Helix Fossil"); sleep(2);
    }
    else if (input == 1) {
        player.key_items[player.numKeyItems] = K_ITEM_FOSSIL_DOME;
        player.numKeyItems++;
        print_to_list("Received the Dome Fossil"); sleep(2);
    }
    player.loc->y += 1; //Set player location to outside
}


void handle_process_fossil() {
    clear();
    begin_list();
    int item_id = 0;
    int item_index = 0;

    if (has_key_item(K_ITEM_FOSSIL_HELIX) >= 0) {
        item_id = K_ITEM_FOSSIL_HELIX;
        item_index = has_key_item(K_ITEM_FOSSIL_HELIX);
    }
    else if (has_key_item(K_ITEM_FOSSIL_DOME) >= 0) {
        item_id = K_ITEM_FOSSIL_DOME;
        item_index = has_key_item(K_ITEM_FOSSIL_DOME);
    }
    else if (has_key_item(K_HELIX_VOUCHER) >= 0) {
        item_id = K_HELIX_VOUCHER;
        item_index = has_key_item(K_HELIX_VOUCHER);
    }
    else if (has_key_item(K_DOME_VOUCHER) >= 0) {
        item_id = K_DOME_VOUCHER;
        item_index = has_key_item(K_DOME_VOUCHER);
    }
    else {
        print_to_list("You don't have a fossil!"); sleep(2);
        player.loc->y += 1;
        return;
    }

    if (item_id == K_ITEM_FOSSIL_DOME || item_id == K_ITEM_FOSSIL_HELIX) {
        take_fossil(item_index, item_id);
    }
    else {
        give_fossil_pokemon(item_index, item_id);
    }

    player.loc->y += 1; //Set player location to outside
}


void take_fossil(int fossil_index, int fossil_type) {
    char print_str[100];
    int id = fossil_type - K_ITEM_EMPTY;

    sprintf(print_str, "Would you like to process your %s?\n  Yes\n  No", key_item_names[id]);
    print_to_list(print_str);
    int input = get_selection(1, 1, 0);

    if (fossil_type == K_ITEM_FOSSIL_DOME) fossil_type = K_DOME_VOUCHER;
    else fossil_type = K_HELIX_VOUCHER;
    id = fossil_type - 200;

    if (input == 0) {           //Yes
        player.key_items[fossil_index] = fossil_type;
        sprintf(print_str, "Received a %s", key_item_names[id]);
        print_to_list(print_str); sleep(2);
    }
    else if (input == 1) {      //No
        print_to_list("Come Back when you're ready!"); sleep(2);
    }
}

void give_fossil_pokemon(int fossil_index, int fossil_type) {
    char print_str[100];
    int id = 0;

    if (fossil_type == K_HELIX_VOUCHER) { id = POKEMON_OMANYTE; }
    else { id = POKEMON_KABUTO; }

    remove_key_item(fossil_type);

    print_to_list("Your fossil was successfully processed!\n \n"); sleep(3);

    Pokemon fossil_pok = *create_new_pokemon(id, 15, 0, 0);

    //If player already has 6 pokemon, transfer the new pokemon to the PC
    if (player.numInParty >= 6) {
        player.pc_storage[player.numInPCStorage] = (fossil_pok);
        player.numInPCStorage++;

        //Reset all stats of that pokemon in the PC
        Pokemon * new_pok = &(player.pc_storage[player.numInPCStorage-1]);
        new_pok->currentHP = new_pok->maxHP;
        new_pok->visible_condition = NO_CONDITION;
        reset_stat_stages(new_pok);
        for (int i = 0; i < new_pok->numAttacks; i++) {
            new_pok->attacks[i].curr_pp = new_pok->attacks[i].max_pp;
        }

        sprintf(print_str, "%s was transferred to PC storage.", fossil_pok.name); 
        print_to_list(print_str); sleep(2);
    }
    //Add Pokemon to party
    else {
        player.party[player.numInParty] = (fossil_pok);
        player.numInParty++;
        sprintf(print_str, "%s was added to your party.", fossil_pok.name);
        print_to_list(print_str); sleep(2);
    }
}