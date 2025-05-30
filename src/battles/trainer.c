#include "trainer.h"

#include <stdio.h>
#include <ncurses.h>

#include "battle.h"
#include "../monsters/pokemon.h"
#include "../player.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"
#include "../audio/audio_player.h"
#include "../motion/map_drawing.h"

#define NAME_MAX_LENGTH 20

static char name[NAME_MAX_LENGTH];

void print_trainer_pokemon(Trainer * trainer, Pokemon * trainer_pokemon);


//Battle a trainer with a random name and random # of pokemon
int battle_trainer(Trainer * trainer) {
    char print_str[1024];

    //Print trainer's battle message
    sprintf(print_str, "%s: \"%s\"", trainer->name, trainer->message);
    print_to_message_box(print_str); await_user();

    audio_end_loop();
    audio_play_file("trainer_begin.mp3");

    //Blink screen and reset screen without battle message
    save_print_state();
    blink_screen(10, restore_print_state);       //7        / 10
    begin_message_box(); save_print_state();
    begin_list();
    usleep(550000);                            //1200000    / 550000
    audio_loop_file("trainer_battle.mp3");

    //Do not allow a player with no pokemon to battle a trainer
    if (!player_get_num_alive()) {
        begin_list();
        print_to_list("  All Pokemon have fainted, please heal them.\n");
        sleep(3);
        return BATTLE_WHITE_OUT;
    }

    //Declarations
    int inputNum, battle_result, return_execute;
    Pokemon trainer_pokemon[trainer->num_in_party];
    battle_result = BATTLE_WIN;
    player.is_trainer_battle = true;
    bool switch_chosen = true;

    //Introductory message
    begin_list();
    sprintf(print_str, "  %s wants to battle!\n", trainer->name);
    print_to_list(print_str); sleep(2);

    //Create all trainer pokemon
    for (int i = 0; i < trainer->num_in_party; i++) {
        trainer_pokemon[i] = *create_new_pokemon(trainer->party_id_list[i], trainer->levels[i], 0, 0);
    }

    //Set up first player pokemon
    player_set_current_pokemon(PLAYER_DEFAULT_POKEMON);



    // BEGIN FIGHT TRAINER POKEMON //
    for (int i = 0; i < trainer->num_in_party; i++) {
        //Print trainer info and sent out
        print_trainer_pokemon(trainer, trainer_pokemon); sleep(2);
        sprintf(print_str, "  %s sent out %s\n", trainer->name, trainer_pokemon[i].name);
        print_to_list(print_str); sleep(2);

        //Only display sent out message if player switched a pokemon
        if (switch_chosen) {
            switch_chosen = false;
            sprintf(print_str, " \n  %s sent out %s\n", player.name, player.current_pokemon->nickname);
            print_to_list(print_str); sleep(2);
        }

        //Execute battle, and break out of loop if white out or finished last trainer pokemon
        battle_result = handle_battle(&(trainer_pokemon[i]));
        if (battle_result == BATTLE_WHITE_OUT || i >= trainer->num_in_party - 1) { break; }

        //If player only has one pokemon left alive, they can't switch
        if (player_get_num_alive() == 1) continue;

        //Handle switch request
        begin_list();
        sprintf(print_str, "  %s is about to send out %s\n", trainer->name, trainer_pokemon[i+1].name);
        sprintf(print_str, "%s  Will %s change Pokemon?\n  Yes\n  No\n", print_str, player.name);
        print_to_list(print_str);
        inputNum = get_selection(2, 1, 0);

        //Do not switch if player selected no
        if (inputNum == 1 || inputNum == PRESSED_B) continue;

        //Handle switch
        while (1) {
            begin_list();
            print_to_list("Select a pokemon to use.\n");
            printParty();
            
            inputNum = get_selection(2, player.numInParty-1, 0);

            if (inputNum == PRESSED_B || &(player.party[inputNum]) == player.current_pokemon) {
                break;
            }
            else if (player.party[inputNum].currentHP == 0) {
                move(player.numInParty+3,0);
                print_to_list(" \nYou must select a different pokemon.\n"); sleep(2);
            }
            else {
                reset_stat_stages(player.current_pokemon);
                player_set_current_pokemon(inputNum);
                switch_chosen = true;
                break;
            }
        }
    }
    // END FIGHT TRAINER POKEMON //



    //Handle a victory
    if (battle_result == BATTLE_WIN) {
        audio_loop_file("trainer_victory.mp3");

        //Notify victory
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("%s defeated %s", player.name, trainer->name);
        refresh(); sleep(2);

        //Calculate money earned
        int money_earned = 0;
        for (int i = 0; i < trainer->num_in_party; i++) {
            money_earned += (trainer->levels[i] * 5) - (rand() % 5);
        }
        money_earned *= 5;

        if (trainer->id_num > 200) money_earned *= 3; //Gym leadeers give 3 times as much

        //Notify money earned
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("%s gained $%d for ", player.name, money_earned);
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("defeating %s",  trainer->name);
        player.money += money_earned;
        refresh(); await_user();

        //Notify badge earned if gym leader
        if (trainer->id_num > 200 && trainer->id_num <= 208) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("Congratulations! %s received badge #%d", player.name, trainer->id_num - 200);
            text_box_cursors(TEXT_BOX_NEXT_LINE);
            printw("from %s",  trainer->name); refresh(); await_user();
        }
    }

    //Reset all stat stages of player pokemon
    for (int i = 0; i < player.numInParty; i++) {
        reset_stat_stages(&(player.party[i]));
    }

    //Free all memmory allocated for memory pokemon
    for (int i = 0; i < trainer->num_in_party; i++) {
        destroy_pokemon(&(trainer_pokemon[i]));
    }

    player.is_battle = false;
    player.is_trainer_battle = false;
    
    //Handle return values
    if (battle_result == BATTLE_WHITE_OUT) {
        init_map();
        return BATTLE_WHITE_OUT;
    }
    else {
        player_add_battled_trainer(trainer->id_num);
        return BATTLE_WIN;
    }
}


void print_trainer_pokemon(Trainer * trainer, Pokemon * trainer_pokemon) {
    char print_str[128];
    begin_list();
    sprintf(print_str, "  %s Pokemon: ", trainer->name);

    for (int i = 0; i < trainer->num_in_party; i++) {
        if (trainer_pokemon[i].currentHP > 0) 
            sprintf(print_str, "%s o", print_str);
        else
            sprintf(print_str, "%s x", print_str);
    }

    sprintf(print_str, "%s\n", print_str);
    print_to_list(print_str);
}


//Get a random name (could be female or male)
char * get_random_name() {
    FILE *og_file;
    // Open the file for reading
    og_file = fopen("names.txt", "r");
    char line[50];

    // Check if the file was opened successfully
    if (og_file == NULL) {
        printw("Name file not found.\n"); refresh(); sleep(2);
        return 1;
    }

    int name_pos = rand() % 2781;
    int count = 0;

    while (count < name_pos) {
        fgets(line, NAME_MAX_LENGTH, og_file);
        sscanf(line, "%s", name);
        count++;
    }

    return name;
}