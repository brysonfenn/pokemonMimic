#include "npc.h"

#include "../items/items.h"
#include <stdio.h>
#include "../print/print_defines.h"

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

