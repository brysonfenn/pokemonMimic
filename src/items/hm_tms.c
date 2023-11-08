#include "hm_tms.h"

#include <stdio.h>
#include <string.h>

#include "../player.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../monsters/pokemon.h"
#include "../monsters/attacks.h"


static char key_item_names[10][32] = {"Empty HM/TM", "HM01 Cut" };


char * get_hm_tm_name(int id) {
    return key_item_names[id-HM_EMPTY];
}

//Returns false if player already has the hm/tm
bool add_hm_tm(HM_TM id) {
    for (int i = 0; i < player.numHMTMs; i++) {
        if (player.hm_tms[i] == id) return false;
    }

    player.hm_tms[player.numHMTMs] = id;
    player.numHMTMs++;
    return true;
}


void teach_hm_tm(HM_TM id) {
    bool able_array[6] = {false, false, false, false, false, false};
    char print_str[64];
    char line[LINE_SIZE];
    int hm_tm_num, attack_num_index;
    char * hm_tm_str;
    char name[64];
    FILE *fp;

    // Open the file for reading
    fp = fopen("hm_tm.txt", "r");
    // Check if the file was opened successfully
    if (fp == NULL) {
        print_to_list("hm_tm.txt does not exist"); sleep(2);
        return;
    }

    //Get correct line information
    while (1) {
        fgets(line, LINE_SIZE, fp);
        sscanf(line, "%d", &hm_tm_num);
        if (hm_tm_num == id) break;
        if (hm_tm_num == 777) return;
    }
    sscanf(line, "%d: %d, %s", &hm_tm_num, &attack_num_index, name);

    //Find where pokemon_id numbers start
    int i = 0;
    for (i = 0; i < strlen(line); i++) if (line[i] == '~') break;
    hm_tm_str = &(line[i+2]);

    while (sscanf(hm_tm_str, "%d", &id) == 1) {
        //Check which pokemon in party can learn the move
        for (int i = 0; i < player.numInParty; i++) {
            Pokemon * curr_pok = &(player.party[i]);
            if (curr_pok->id_num == id) {
                able_array[i] = true;

                //Pokemon cannot learn same move twice
                for (int j = 0; j < curr_pok->numAttacks; j++) {
                    if (curr_pok->attacks[j].id_num == attack_num_index)
                        able_array[i] = false;
                }
            }
        }

        // Move to the next number in the string
        hm_tm_str = strchr(hm_tm_str, ' ');
        if (hm_tm_str == NULL) {
            break;
        }
        hm_tm_str++;	//Move up a space
    }

    //Get player selection based on able/unable list
    begin_list();
    sprintf(print_str, "Select a Pokemon to teach %s:", name);
    print_to_list(print_str);
    print_party_able_unable(able_array);
    print_to_list("  Cancel");
    int selected_pok = get_selection(1, player.numInParty, 0);

    //Handle Selection, and learn move if it is possible
    if (selected_pok == player.numInParty || selected_pok == PRESSED_B) { return; }   //Cancel
    else if (!able_array[selected_pok]) {
        sprintf(print_str, "%s cannot learn %s", player.party[selected_pok].name, name);
        begin_list(); print_to_list(print_str); sleep(2);
        return;
    }
    learn_move(&(player.party[selected_pok]), get_attack_by_id(attack_num_index));
    return;
}