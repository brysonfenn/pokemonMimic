#ifndef SELECTABLES_H
#define SELECTABLES_H

#include <stdbool.h>

#define SELECTABLE_CONTINUE_WHILE 1
#define SELECTABLE_BREAK_WHILE 2

enum { SELECTABLE_NONE, SELECTABLE_TRAINER, SELECTABLE_NPC, SELECTABLE_CUTTABLE_TREE, SELECTABLE_SNORLAX, SELECTABLE_SURF

};

typedef struct Selectable {
    int x;
    int y;
    int selectable_id;
    void * data;
} Selectable;


struct Trainer;

//Add a trainer at a given location
void add_trainer_by_id(char x, char y, int trainer_id, char face_direction);

//Add npc at a given location
void add_npc_by_id(char x, char y, int npc_id, char face_direction);

//Add Cuttable Tree at a location
void add_cuttable_tree(char x, char y);

//Add Surf selectable
void add_surf_selectable(char x, char y);

//Add Cuttable Tree at a location
void add_snorlax(char x, char y, char record_bit_num);

// Return a selectable
Selectable * get_selectable(int player_x, int player_y, char player_char);

//Erase selectable list
void clear_selectables();

//Return a selectable containing a trainer if it is within battle distance of the player
Selectable * get_triggered_selectable(int player_x, int player_y, int *x_inc, int *y_inc, char * next_player_char);



// selectables_handle.c

// Handle a selected selectable
int handle_selected_selectable(int player_x, int player_y, char player_char);

#endif //SELECTABLES_H