#include "npc.h"

#include <stdio.h>

#include "location.h"
#include "motion2d.h"
#include "../player.h"
#include "../items/items.h"
#include "../items/hm_tms.h"
#include "../items/key_items.h"
#include "../print/print_defines.h"
#include "../monsters/pokemon.h"


#define GIVE_POK_START_INDEX 300

#define NUM_NPCS 40
char message[LINE_SIZE];

enum {NPC_ACTION_NONE, NPC_ACTION_RESET_MAP, NPC_ACTION_SWITCH_R_BIT };

static NPC n00 = {  0, "No Name", NO_ITEM, 0, NPC_ACTION_NONE};
static NPC n01 = {  1, "Finn", FIRE_STONE, RECORD_BIT_1, NPC_ACTION_NONE};
static NPC n02 = {  2, "Monty", MOON_STONE, RECORD_BIT_2, NPC_ACTION_NONE};
static NPC n03 = {  3, "Bill", K_ITEM_SS_TICKET, RECORD_BIT_3, NPC_ACTION_NONE };
static NPC n04 = {  4, "Guard", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n05 = {  5, "Guard", NO_ITEM, RECORD_BIT_8, NPC_ACTION_RESET_MAP };
static NPC n06 = {  6, "Captain Jacobs", HM_CUT, RECORD_BIT_4, NPC_ACTION_NONE };
static NPC n07 = {  7, "Lily", LEAF_STONE, RECORD_BIT_5, NPC_ACTION_NONE };
static NPC n08 = {  8, "Ted", K_ITEM_FOSSIL_AMBER, RECORD_BIT_6, NPC_ACTION_NONE };
static NPC n09 = {  9, "Calvin", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n10 = { 10, "Taylor", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n11 = { 11, "Jan", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n12 = { 12, "Finn", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n13 = { 13, "Locklyn", K_ITEM_SILPH_SCOPE, RECORD_BIT_9, NPC_ACTION_RESET_MAP };
static NPC n14 = { 14, "Guard", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n15 = { 15, "Guard", NO_ITEM, RECORD_BIT_10, NPC_ACTION_RESET_MAP };
static NPC n16 = { 16, "Pokemon League Rep", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n17 = { 17, "Officer", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n18 = { 18, "Mr. Fuji", K_ITEM_FLUTE, RECORD_BIT_11, NPC_ACTION_RESET_MAP };
static NPC n19 = { 19, "Ella", GIVE_POK_START_INDEX + POKEMON_EEVEE, RECORD_BIT_14, NPC_ACTION_NONE };
static NPC n20 = { 20, "Pokemon League Rep", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n21 = { 21, "Safari Ranger", NO_ITEM, 0, NPC_ACTION_NONE};
static NPC n22 = { 22, "Safari Manager", HM_SURF, RECORD_BIT_13, NPC_ACTION_NONE };
static NPC n23 = { 23, "Pokemon League Rep", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n24 = { 24, "Pokemon League Rep", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n25 = { 25, "Rocket Grunt", NO_ITEM, 0, NPC_ACTION_NONE };
static NPC n26 = { 26, "Silph Co. Worker", K_ITEM_KEY_SILPH, RECORD_BIT_16, NPC_ACTION_NONE };
static NPC n27 = { 27, "Silph Co. CEO", MASTER_BALL, RECORD_BIT_15, NPC_ACTION_RESET_MAP };
static NPC n28 = { 28, "Silph Co. Researcher", THUNDER_STONE, RECORD_BIT_17, NPC_ACTION_NONE };
static NPC n29 = { 29, "Statue", NO_ITEM, RECORD_BIT_18, NPC_ACTION_SWITCH_R_BIT };
static NPC n30 = { 30, "Cinnabar Serviceman", K_ITEM_SECRET_KEY, RECORD_BIT_19, NPC_ACTION_NONE };
static NPC n31 = { 31, "Dr. Susenhogen", REVIVE, RECORD_BIT_20, NPC_ACTION_NONE };
static NPC n32 = { 32, "Terrence", HYPER_POTION, RECORD_BIT_21, NPC_ACTION_NONE };
static NPC n33 = { 33, "Mansion Owner Sam", WATER_STONE, RECORD_BIT_22, NPC_ACTION_NONE };


static NPC * npcs[NUM_NPCS] = 
    { &n00, &n01, &n02, &n03, &n04, &n05, &n06, &n07, &n08, &n09, &n10, &n11, &n12, &n13, &n14, &n15, &n16, &n17, &n18, &n19, &n20,
            &n21, &n22, &n23, &n24, &n25, &n26, &n27, &n28, &n29, &n30, &n31, &n32, &n33 };


struct NPC * get_npc(int npc_id) {
    return npcs[npc_id];
}

char * get_npc_message(int npc_id) {

    FILE *fp;

    char line[LINE_SIZE];
    int line_num;

    // Open the file for reading
    fp = fopen("npc_messages.txt", "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        printw("NPC messages file does not exist.\n"); refresh(); sleep(2);
        return;
    }

    while (fgets(line, LINE_SIZE, fp)) {
        sscanf(line, "%d: %[^\n]", &line_num, message);
        if (line_num == npc_id) return message;
        if (line_num == 777) return message;
    }
}


//Handle the case where the player select an NPC
void handle_npc_selection(struct NPC * npc_ptr) {
    char print_str[2048];
    char item_name[32];
    Pokemon new_pok;

    sprintf(print_str, "%s: %s", npc_ptr->name, get_npc_message(npc_ptr->id_num));
    print_to_message_box(print_str);
    
    // Check if this NPC has already given an item, if not, give item
    if (npc_ptr->givable_item != NO_ITEM) {
        await_user();

        //if givable item is a pokemon
        if (npc_ptr->givable_item >= GIVE_POK_START_INDEX) {
            new_pok = *create_new_pokemon(npc_ptr->givable_item - GIVE_POK_START_INDEX, 25, 0, 0);
            sprintf(item_name, "%s", new_pok.nickname);
        }
        //if givable item is a key item
        else if (npc_ptr->givable_item >= K_ITEM_EMPTY) {
            sprintf(item_name, "%s", get_key_item_name(npc_ptr->givable_item));
        }
        //else if givable item is an HM/TM
        else if (npc_ptr->givable_item >= HM_EMPTY) {
            sprintf(item_name, "%s", get_hm_tm_name(npc_ptr->givable_item));
        }
        //else if givable item is a regular item
        else {
            sprintf(item_name, "%s", get_item_by_id(npc_ptr->givable_item)->name);
        }

        //Check if player has already received an item from the NPC
        if ((player.record_bits >> (npc_ptr->record_bit_num)) & 0x01) {
            sprintf(print_str, "%s: I already gave you my %s", npc_ptr->name, item_name);
            print_to_message_box(print_str);
        }
        else {
            sprintf(print_str, "%s: Here, take my %s!", npc_ptr->name, item_name);
            print_to_message_box(print_str); sleep(2);
            sprintf(print_str, "%s received %s!", player.name, item_name);
            print_to_message_box(print_str);

            //Give item
            if (npc_ptr->givable_item >= GIVE_POK_START_INDEX) give_pokemon_to_player(&new_pok);
            if (npc_ptr->givable_item >= K_ITEM_EMPTY) add_key_item(npc_ptr->givable_item);
            else if (npc_ptr->givable_item >= HM_EMPTY) add_hm_tm(npc_ptr->givable_item);
            else give_item_to_player(get_item_by_id(npc_ptr->givable_item), 1);
            
        }
    }

    await_user();
    begin_message_box();

    //Record that npc has been talked to
    if (npc_ptr->record_bit_num != 0) {
        
        switch (npc_ptr->action) {
        case NPC_ACTION_RESET_MAP:
            if (player_set_record_bit(npc_ptr->record_bit_num)) {
                change_map(player.loc->map, player.loc->x, player.loc->y);
            }  
            break;
        case NPC_ACTION_SWITCH_R_BIT:
            player_switch_record_bit(npc_ptr->record_bit_num);
            change_map(player.loc->map, player.loc->x, player.loc->y);
            break;
        case NPC_ACTION_NONE:
        default:
            player_set_record_bit(npc_ptr->record_bit_num);
            break;
        }
    }

    usleep(500000);
}

