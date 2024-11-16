#include "safari_zone.h"

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "battle.h"
#include "../player.h"
#include "../monsters/pokemon.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../print/print_battle.h"
#include "../items/items.h"
#include "../motion/motion2d.h"
#include "../motion/maps.h"
#include "../audio/audio_player.h"

static enum decision {NONE, BALL, BAIT, APPROACH, RUN } current_decision = NONE;

int safari_ball_count = 0;
int bait_count = 0;

void print_safari_battle(int distance);

void setup_safari_zone() {
    safari_ball_count = 30;
    bait_count = 30;
}

//Begin a Battle with a pokemon in the safari zone
int safari_zone_encounter(struct Pokemon * enemyPoke) {
    int input_num, last_selection = 0;
    int distance = 50;
    int flee_chance = 50; //Percentage
    int hit_chance = 50; //
    
    player.enemy_pokemon = enemyPoke;
    player.is_battle = true;
    current_decision = NONE;

    ///////// GET DECISION /////////
    while (current_decision != RUN) {
        clear();
        print_safari_battle(distance);

        //Get Decision
        mvprintw(SELECT_Y,BATTLE_SELECT_1_X,"  Ball"); mvprintw(SELECT_Y,BATTLE_SELECT_2_X,"  Bait");
        mvprintw(SELECT_Y+1,BATTLE_SELECT_1_X,"  Approach"); mvprintw(SELECT_Y+1,BATTLE_SELECT_2_X,"  Run");
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("What will %s do?", player.name);

        input_num = get_battle_selection(SELECT_Y, last_selection);
        last_selection = input_num;
        current_decision = input_num + 1; // In this case, the decision will be one more than selection

        switch (current_decision) {
            
            case BALL:
                text_box_cursors(TEXT_BOX_BEGINNING);

                if (safari_ball_count > 0) {
                    printw("%s threw a Safari Ball!", player.name); refresh(); sleep(2);
                    safari_ball_count--;

                    //Ball Hits - Attempt Catch
                    if (rand() % 100 < hit_chance) {
                        //TODO: HANDLE CATCH - Then add pokemone and return
                    }
                    //Ball Misses
                    else {
                        text_box_cursors(TEXT_BOX_BEGINNING);
                        printw("It Missed!"); refresh(); sleep(2);
                    }
                }
                else {
                    printw("Out of Safari Balls"); refresh(); sleep(2);
                }
                break;
                
            case BAIT:
                //Decrease Flee Chance
                text_box_cursors(TEXT_BOX_BEGINNING);
                
                if (bait_count > 0) {
                    printw("%s threw some bait!", player.name); refresh(); sleep(2);
                    bait_count--;
                }
                else {
                    printw("Out of Bait"); refresh(); sleep(2);
                }

                if (flee_chance > 10) {
                    flee_chance -= 10;
                }
                break;
                
            case APPROACH:
                //Increase Flee Chance, Increase Catch Chance
                text_box_cursors(TEXT_BOX_BEGINNING);
                printw("%s approached the %s!", player.name, enemyPoke->nickname); refresh(); sleep(2);
                distance -= 10;
                hit_chance += 10;
                if (distance <= 0) {
                    flee_chance = 100;
                }
                break;
                
            case RUN:
                break;
            default:
                current_decision = NONE;
                break;
        }

        if ((rand() % 100) < flee_chance) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("The %s fled!", enemyPoke->nickname); refresh(); await_user();
            return BATTLE_WIN;
        }
    }
    ///////// END GET DECISION /////////

    return BATTLE_WIN;
}


void print_safari_battle(int distance) {
    Pokemon * enemy_pok = player.enemy_pokemon;
    char print_str[128];

    clear();

    //Draw battle box
    draw_box(BATTLE_BOX_X, BATTLE_BOX_Y, BATTLE_BOX_WIDTH, BATTLE_BOX_HEIGHT);

    mvprintw(BATTLE_BOX_PLAYER_Y, BATTLE_BOX_PLAYER_X, "%s", player.name);
    mvprintw(BATTLE_BOX_PLAYER_Y+1, BATTLE_BOX_PLAYER_X, "Safari Balls: %d -- Bait: %d", safari_ball_count, bait_count);

    //Display Indication if Pokemon is uncaught
    if (player.is_uncaught_pokemon) {
        mvprintw(BATTLE_BOX_ENEMY_Y, BATTLE_BOX_ENEMY_X - 4, "*");
    }

    mvprintw(BATTLE_BOX_ENEMY_Y, BATTLE_BOX_ENEMY_X, "%s  Lvl %d  ", player.enemy_pokemon->nickname, player.enemy_pokemon->level);
    mvprintw(BATTLE_BOX_ENEMY_Y+1, BATTLE_BOX_ENEMY_X, "Distance: %d Feet", distance);

    print_btn_instructions(false);

    //Draw text box
    draw_box(TEXT_BOX_X, TEXT_BOX_Y, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);
    refresh();
}