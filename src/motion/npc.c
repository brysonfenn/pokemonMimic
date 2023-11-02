#include "npc.h"

#include "../items/items.h"

#define NUM_NPCS 30

static NPC n0 = { 000, "No Name", "I have no name", NO_ITEM };
static NPC n1 = { 001, "Johnny", "This is a test message", NO_ITEM };


static NPC * npcs[NUM_NPCS] = { &n0, &n1 };

struct NPC * get_npc(int npc_id) {
    return npcs[npc_id];
}

