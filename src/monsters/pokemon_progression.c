#include "pokemon.h"


#include <stdio.h>
#include <ncurses.h>

#include "../player.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"
#include "../audio/audio_player.h"

int evolve(Pokemon * pok, int next_pok_id);
int handle_evolve_in_battle(Pokemon * pok, int next_pok_id);
void handle_learnset(Pokemon * pok);

//Handle leveling up - also handles learning moves from new level
void pokemon_level_up(Pokemon *pok, int next_level_exp) {
    audio_save_looping_file(1);
    audio_loop_file("level_up.mp3");

    //Pokemon over level 100 should not be given exp
    if (pok->level >= 100) return;

    pok->level++;
    int lost_hp = pok->maxHP - pok->currentHP;

    //Redo stat calculation
    Pokemon frame = *(get_pokemon_frame(pok->id_num));
    frame.iv = pok->iv;
    frame.ev = pok->ev;
    calculate_stats(&frame, pok->level, 0, 0);

    //Set all pokemon stats to the new calculated stats
    pok->maxHP = frame.maxHP;
    pok->currentHP = pok->maxHP - lost_hp;
    pok->baseAttack = frame.baseAttack;     //Keep lost hp the same
    pok->baseDefense = frame.baseDefense;
    pok->baseSpeed = frame.baseSpeed;

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("%s has grown to level %d!", pok->nickname, pok->level);
    refresh(); sleep(3);
    audio_restore_looping_file(1);

    //learn_moves again
    int level_target, evolve_id;
    handle_learnset(pok);
    get_evolve_level_and_id(pok->id_num, &level_target, &evolve_id);

    //If evolution id is 0, this pokemon has no evolves
    if (evolve_id == 0) { return; } 
    //Evolve the pokemon and learn any new moves if pokemon has reached evolution level or higher
    else if (pok->level >= level_target) {
        handle_evolve_in_battle(pok, evolve_id);
        handle_learnset(pok);
    }
}


//Return exp required for Pokemon pok to reach the next level
uint32_t pokemon_get_next_level_exp(Pokemon *pok) {
    uint32_t next_level = pok->level + 1;
    return (next_level * next_level * next_level);  //Next level exp = n^3
}


//Return exp yield of a defeated pokemon
uint32_t pokemon_get_exp_yield(Pokemon *pok) {
    uint32_t avg_stat = pok->baseAttack + pok->baseDefense + pok->baseSpeed;
    avg_stat += pok->baseSpAttack + pok->baseSpDefense;
    avg_stat /= 5;

    //Equation chosen for yield is ((avg_stat + level / 5)^2) / 3
    uint32_t exp_yield = (avg_stat + (pok->level / 5));
    exp_yield = (exp_yield * exp_yield) / 2;
    return exp_yield;
}


//Cause a pokemon to learn moves at its current level
void handle_learnset(Pokemon * pok) {
    char line[LINE_SIZE];
    char print_str[256];

    //Add moves
    FILE *fp;
    char filename[50];
    sprintf(filename, "learnsets/id_%03d.txt", pok->id_num);

    // Open the file for reading
    fp = fopen(filename, "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        printw("Learnset file does not exist.\n"); refresh(); sleep(2);
        return;
    }

    // Read lines from the file and put them into game values
    fgets(line, LINE_SIZE, fp);	// Name first line
    fgets(line, LINE_SIZE, fp);  // Evolve second line
    fgets(line, LINE_SIZE, fp);	// Move Format third line

    attack new_attack;
    int level_target, move_id;

    while (fgets(line, LINE_SIZE, fp)) {
        // if (strstr(line, "end") != NULL) break;
        sscanf(line, "%d,%d", &level_target, &move_id);
        if (level_target == pok->level) {
            new_attack = *(get_attack_by_id(move_id));
            learn_move(pok, &new_attack);
        }
    }

    fclose(fp);
    }


    //Put evolution level for pokemon with ID # pok_id in level_target and ID # of evolution in evolve_id
    void get_evolve_level_and_id(int pok_id, int * level_target, int * evolve_id) {
    char line[LINE_SIZE];
    char print_str[256];

    //Add moves
    FILE *fp;
    char filename[50];
    sprintf(filename, "learnsets/id_%03d.txt", pok_id);

    // Open the file for reading
    fp = fopen(filename, "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        printw("Learnset file does not exist.\n"); refresh(); sleep(2);
        return;
    }

    // Read lines from the file and put them into game values
    fgets(line, LINE_SIZE, fp);	// Name first line
    fgets(line, LINE_SIZE, fp);  // Evolve second line

    sscanf(line, "Evolve: %d, %d", level_target, evolve_id);
}


//Give a single move to a pokemon - also handle forgetting moves
void learn_move(Pokemon * pok, attack * new_attack) {
    int input_num;

    //If the pokemon already knows this attack, do not learn
    for (int i = 0; i < pok->numAttacks; i++) {
        if (pok->attacks[i].id_num == new_attack->id_num) return;
    }

    new_attack->curr_pp = new_attack->max_pp;

    //If there are not 4 moves, auto-learn, if not, forget.
    if (pok->numAttacks < 4) {
        pok->attacks[pok->numAttacks] = *new_attack;
        pok->numAttacks++;
    }
    else {

        bool made_selection = false;

        while (!made_selection) {
            mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  %s", pok->attacks[0].name); 
            mvprintw(SELECT_Y,BATTLE_SELECT_2_X,"  %s", pok->attacks[1].name); 
            mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  %s", pok->attacks[2].name); 
            mvprintw(SELECT_Y+1,BATTLE_SELECT_2_X,"  %s", pok->attacks[3].name); 
            mvprintw(SELECT_Y+1,BATTLE_SELECT_3_X,"  Cancel");

            text_box_cursors(TEXT_BOX_BEGINNING); 
            text_box_cursors(TEXT_BOX_NEXT_LINE); text_box_cursors(TEXT_BOX_NEXT_LINE);
            printw("%s wants to learn %s!", pok->nickname, new_attack->name); 
            refresh(); sleep(2);

            text_box_cursors(TEXT_BOX_NEXT_LINE);
            printw("Select a move to forget.");
                
            input_num = get_move_selection(BATTLE_SELECT_1_X, SELECT_Y, pok, 0);
            if (input_num >= 0 && input_num <= 3) {
                text_box_cursors(TEXT_BOX_BEGINNING);
                printw("Forget %s?", pok->attacks[input_num].name);
                text_box_cursors(TEXT_BOX_NEXT_LINE); printw("Yes");
                text_box_cursors(TEXT_BOX_NEXT_LINE); printw("No");
                made_selection = !get_selection(BATTLE_BOX_Y+BATTLE_BOX_HEIGHT+4, 1, 0);
            }
            else {
                made_selection = true;
            }
        }

        if (player.is_battle) {
            clear(); printBattle();
        }

        text_box_cursors(TEXT_BOX_BEGINNING);

        if (input_num == 4 || input_num == PRESSED_B) {
            printw("%s did not learn %s!", pok->nickname, new_attack->name); refresh(); sleep(2); return;
        }

        printw("1...2...and...poof!"); refresh(); sleep(2);

        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("%s forgot %s, and...", pok->nickname, pok->attacks[input_num]); refresh(); sleep(2);
        pok->attacks[input_num] = *new_attack;
    }

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("%s learned %s!", pok->nickname, new_attack->name); refresh(); sleep(2);
    
}


//Give highest possible level moves to a pokemon
//This is called upon initializing a new pokemon
void pokemon_give_moves(Pokemon *pok) {
    pok->numAttacks = 0;

    //Add moves
    FILE *fp;
    char filename[50];
    sprintf(filename, "learnsets/id_%03d.txt", pok->id_num);
    char line[LINE_SIZE];

    // Open the file for reading
    fp = fopen(filename, "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        printw("Learnset file does not exist.\n"); refresh(); sleep(2);
        return;
    }

    // Read lines from the file and put them into game values
    fgets(line, LINE_SIZE, fp);	// Name first line
    fgets(line, LINE_SIZE, fp);  // Evolve second line
    fgets(line, LINE_SIZE, fp);	// Format second line

    int level_target, move_id;
    attack new_attack;
    attack * pok_attack;
    uint8_t attack_position = 0;

    while (fgets(line, LINE_SIZE, fp)) {
        // if (strstr(line, "end") != NULL) break;
        sscanf(line, "%d,%d", &level_target, &move_id);

        if (level_target <= pok->level) {
            //Don't get a move twice
            bool double_attack = false;
            for (int i = 0; i < pok->numAttacks; i++) {
                if (pok->attacks[i].id_num == move_id)
                double_attack = true;
            }
            if (double_attack) continue;

            new_attack = *(get_attack_by_id(move_id));
            pok_attack = &(pok->attacks[attack_position]);

            *(pok_attack) = new_attack;
            pok_attack->curr_pp = pok_attack->max_pp;
            attack_position++;

            if (pok->numAttacks < 4) pok->numAttacks++;   //Only 4 attacks can exist
            if (attack_position > 3) attack_position = 0; //Cycle through position to get highest level attacks
        }
    }

    //Fill in with empty attacks if less than 4
    if (pok->numAttacks < 4) {
        for (int i = pok->numAttacks; i < 4; i++) {
            pok->attacks[i] = *(get_attack_by_id(0));
        }
    }

    fclose(fp);
}


int handle_evolve_in_battle(Pokemon * pok, int next_pok_id) {
    char og_pok_name[LINE_SIZE];
    sprintf(og_pok_name, "%s", pok->nickname);  //Preserve some information like name
    int ch;

    clear();
    printBattle();
    text_box_cursors(TEXT_BOX_BEGINNING); printw("What? %s is evolving!", og_pok_name);
    text_box_cursors(TEXT_BOX_NEXT_LINE); printw("Press '%c' to cancel ", CANCEL_CHAR); refresh();
    audio_save_looping_file(2);
    audio_end_loop();
    audio_play_file("evolution_theme.mp3");

    int count = 0;
    bool pressed_b = false;
    nodelay(stdscr, TRUE);

    while (1) {
        ch = getch();
        if (ch == CANCEL_CHAR || ch == CANCEL_CHAR_2) {
            pressed_b = true;
            break;
        }

        if (count > 7000) break;
        count++;

        if (!(count % 500)) {
            printw(". ");
        }
        
        usleep(1000); //Sleep 1 ms

    }
    usleep(500000);
    nodelay(stdscr, FALSE);
    if (pressed_b) {
        audio_end_play();
        audio_restore_looping_file(2);
        text_box_cursors(TEXT_BOX_NEXT_LINE); printw("%s did not evolve!", og_pok_name); refresh(); sleep(2);
        return 1;
    }

    audio_play_file("evolution_complete.mp3");

    evolve(pok, next_pok_id);

    text_box_cursors(TEXT_BOX_BEGINNING); printw("Congratulations! %s evolved", og_pok_name);
    text_box_cursors(TEXT_BOX_NEXT_LINE); printw("into %s!", pok->name); refresh(); sleep(3);

    //Nickname Pokemon
    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("Would you like to give a nickname to %s?", pok->name);
    text_box_cursors(TEXT_BOX_NEXT_LINE); printw("Yes");
    text_box_cursors(TEXT_BOX_NEXT_LINE); printw("No");
    
    int answer = get_selection(BATTLE_BOX_Y+BATTLE_BOX_HEIGHT+4, 1, 0);
    if (answer == 0) sprintf(pok->nickname, "%s", get_name_input(pok->name));
    else sprintf(pok->nickname, "%s", pok->name);
    
    if (strcmp(pok->nickname, DEFAULT_NAME_STR) == 0) sprintf(pok->nickname, "%s", pok->name);

    audio_restore_looping_file(2);

    return 0;
}


void handle_evolve_outside_battle(Pokemon * pok, int next_pok_id) {
    char print_str[256];
    char og_pok_name[LINE_SIZE];
    sprintf(og_pok_name, "%s", pok->nickname);  //Preserve some information like name
    
    begin_list();
    sprintf(print_str, "What? %s is evolving!", og_pok_name);
    print_to_list(print_str); 
    audio_play_file("evolution_theme.mp3"); usleep(13400000);
    audio_play_file("evolution_complete.mp3");
    
    evolve(pok, next_pok_id);

    sprintf(print_str, " \nCongratulations! %s evolved into %s!", og_pok_name, pok->name);

    print_to_list(print_str); sleep(3);

    
    //Nickname Pokemon
    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("Would you like to give a nickname to %s?", pok->name);
    text_box_cursors(TEXT_BOX_NEXT_LINE); printw("Yes");
    text_box_cursors(TEXT_BOX_NEXT_LINE); printw("No");
    
    int answer = get_selection(BATTLE_BOX_Y+BATTLE_BOX_HEIGHT+4, 1, 0);
    if (answer == 0) sprintf(pok->nickname, "%s", get_name_input(pok->name));
    else sprintf(pok->nickname, "%s", pok->name);

    if (strcmp(pok->nickname, DEFAULT_NAME_STR) == 0) sprintf(pok->nickname, "%s", pok->name);
}


//Return 0 is evolve success, return 1 if failed
int evolve(Pokemon * pok, int next_pok_id) {
    //Preserve some information like lost hp
    int lost_hp = pok->maxHP - pok->currentHP;

    //Get pokemon evolution frame
    Pokemon evolution = *(get_pokemon_frame(next_pok_id));

    //Attach name, base stats of evolution to pokemon, then calculate stats
    sprintf(pok->name, "%s", evolution.name);
    pok->id_num = next_pok_id;
    pok->maxHP = evolution.maxHP; pok->baseAttack = evolution.baseAttack;
    pok->baseDefense = evolution.baseDefense; pok->baseSpAttack = evolution.baseSpAttack;
    pok->baseSpDefense = evolution.baseSpDefense; pok->baseSpeed = evolution.baseSpeed;
    calculate_stats(pok, pok->level, 0, 0);

    //Pokemon should gain typings of evolution
    pok->type1 = evolution.type1;
    pok->type2 = evolution.type2;

    //Pokemon should have the same hp loss
    pok->currentHP = pok->maxHP - lost_hp;

    return 0;
}


//Return iv field
int pokemon_get_iv(Pokemon * pok, char type) {

    //Each iv occupies 5 bits of the iv random number
    switch (type) {
        case IV_HP:
            return ((pok->iv >> 0) & 0x1F);
            break;
        case IV_ATTACK:
            return ((pok->iv >> 5) & 0x1F);
            break;
        case IV_DEFENSE:
            return ((pok->iv >> 10) & 0x1F);
            break;
        case IV_SP_ATTACK:
            return ((pok->iv >> 15) & 0x1F);
            break;
        case IV_SP_DEFENSE:
            return ((pok->iv >> 20) & 0x1F);
            break;
        case IV_SPEED:
            return ((pok->iv >> 25) & 0x1F);
            break;
        default:
            break;
    }

    return 0;
}


//Return ev field
int pokemon_get_ev(Pokemon * pok, char type) {

    //Each ev occupies 8 bits of the ev number
    switch (type) {
        case IV_HP:
            return ((pok->ev >> 0) & 0xFF);
            break;
        case IV_ATTACK:
        case IV_SP_ATTACK:
            return ((pok->ev >> 8) & 0xFF);
            break;
        case IV_DEFENSE:
        case IV_SP_DEFENSE:
            return ((pok->ev >> 16) & 0xFF);
            break;
        case IV_SPEED:
            return ((pok->ev >> 24) & 0xFF);
            break;
        default:
            return -1;
            break;
    }

    return 0;
}


//Increment ev for a pokemon
int pokemon_increment_ev(Pokemon * pok, char type) {
    int inc_val = 0;

    //Each ev occupies 8 bits of the ev number
    switch (type) {
        case IV_HP:
            inc_val = 1 << 0;
            break;
        case IV_ATTACK:
        case IV_SP_ATTACK:
            inc_val = 1 << 8;
            break;
        case IV_DEFENSE:
        case IV_SP_DEFENSE:
            inc_val = 1 << 16;
            break;
        case IV_SPEED:
            inc_val = 1 << 24;
            break;
        default:
            return -1;
            break;
    }

    pok->ev += inc_val;

    return 0;
}