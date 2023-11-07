#include "hm_tms.h"

#include <stdio.h>
#include <string.h>

#include "../player.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../monsters/pokemon.h"
#include "../monsters/attacks.h"


static char key_item_names[10][32] = {"Empty HM/TM", "Cut" };


char * get_hm_tm_name(int id) {
    return key_item_names[id];
}

//Returns false if player already has the key item
bool add_hm_tm(HM_TM id) {
    player.hm_tms[player.numHMTMs] = id;
    player.numHMTMs++;
    return true;
}


void teach_hm_tm(HM_TM id) {
    bool able_array[6] = {false, false, false, false, false, false};
    char print_str[64];

    char line[LINE_SIZE];
    int hm_tm_num;
    char * hm_tm_str;
    int attack_num, temp1;
    char name[64];

    FILE *fp;

    // Open the file for reading
    fp = fopen("hm_tm.txt", "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        print_to_list("hm_tm.txt does not exist"); sleep(2);
        return;
    }

    while (1) {
        fgets(line, LINE_SIZE, fp);
        sscanf(line, "%d", &hm_tm_num);
        if (hm_tm_num == id) break;
        if (hm_tm_num == 777) return;
    }

    sscanf(line, "%d: %d, %s", &temp1, &attack_num, name);

    //Find where numbers start
    int i = 0;
    for (i = 0; i < strlen(line); i++) if (line[i] == '~') break;
    hm_tm_str = &(line[i+2]);

    while (sscanf(hm_tm_str, "%d", &id) == 1) {
        //Check which pokemon can learn the move
        for (int i = 0; i < player.numInParty; i++) {
            Pokemon * curr_pok = &(player.party[i]);
            if (curr_pok->id_num == id) {
                able_array[i] = true;

                //Pokemon cannot learn same move twice
                for (int j = 0; j < curr_pok->numAttacks; j++) {
                    if (curr_pok->attacks[j].id_num == attack_num)
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

    begin_list();
    sprintf(print_str, "Select a Pokemon to teach %s:", name);
    print_to_list(print_str);
    print_party_able_unable(able_array);
    print_to_list("  Cancel");

    int selected_pok = get_selection(1, player.numInParty, 0);
    if (selected_pok == player.numInParty || selected_pok == PRESSED_B) { return; }   //Cancel
    else if (!able_array[selected_pok]) {
        sprintf(print_str, "%s cannot learn %s", player.party[selected_pok].name, name);
        begin_list(); print_to_list(print_str); sleep(2);
        return;
    }

    learn_move(&(player.party[selected_pok]), get_attack_by_id(attack_num));

    return;
}