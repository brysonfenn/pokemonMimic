#ifndef NPC_H
#define NPC_H

#include <stdbool.h>

typedef struct NPC {
    int id_num;
    char name[30];
    int givable_item;
} NPC;


struct NPC * get_npc(int npc_id);

char * get_npc_message(int npc_id);



#endif //NPC_H