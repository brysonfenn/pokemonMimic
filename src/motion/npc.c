#include "npc.h"

#include "../items/items.h"
#include <stdio.h>
#include "../print/print_defines.h"
#include "../player.h"

#define NUM_NPCS 30
char message[LINE_SIZE];

static NPC n0 = { 000, "No Name", NO_ITEM };
static NPC n1 = { 001, "Finn", FIRE_STONE };
static NPC n2 = { 002, "Monty", MOON_STONE };


static NPC * npcs[NUM_NPCS] = { &n0, &n1, &n2 };

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

    sprintf(print_str, "%s: %s", npc_ptr->name, get_npc_message(npc_ptr->id_num));
    print_to_message_box(print_str);
    
    // Check if this NPC has already given an item, if not, give item
    if (npc_ptr->givable_item != NO_ITEM) {
        await_user();
        if ((player.NPCs_done >> (npc_ptr->id_num)) & 0x01) {
            sprintf(print_str, "%s: I already gave you my %s", npc_ptr->name, get_item_by_id(npc_ptr->givable_item)->name);
            print_to_message_box(print_str);
        }
        else {
            sprintf(print_str, "%s: Here, take my %s!", npc_ptr->name, get_item_by_id(npc_ptr->givable_item)->name);
            give_player_item(get_item_by_id(npc_ptr->givable_item), 1);
            print_to_message_box(print_str); sleep(2);
            sprintf(print_str, "%s received %s!", player.name, get_item_by_id(npc_ptr->givable_item)->name);
            print_to_message_box(print_str);
        }
        player.NPCs_done |= ((long long) 1 << npc_ptr->id_num);
    }
    await_user();
    begin_message_box();
    usleep(500000);
}

