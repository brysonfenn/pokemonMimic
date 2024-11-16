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

void print_safari_battle();

void setup_safari_zone() {
    safari_ball_count = 30;
}

//Begin a Battle with a pokemon in the safari zone
int safari_zone_encounter(struct Pokemon * enemyPoke) {
    int input_num, last_selection = 0;
    
    player.enemy_pokemon = enemyPoke;
    player.is_battle = true;
    current_decision = NONE;

    print_safari_battle();

    ///////// GET DECISION /////////
    while (current_decision != RUN) {
        clear();
        printBattle();

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
                printw("%s threw a Safari Ball!", player.name); refresh(); sleep(2);
                break;
                
            case BAIT:
                //Decrease Flee Chance
                
                break;
                
            case APPROACH:
                //Increase Flee Chance, Increase Catch Chance

                break;
                
            case RUN:
                break;
            default:
                current_decision = NONE;
                break;
        }
    }
    ///////// END GET DECISION /////////

    return BATTLE_WIN;
}


void print_safari_battle() {
    Pokemon * enemy_pok = player.enemy_pokemon;
    char poke_string[128];

    clear();

    //Draw battle box
    draw_box(BATTLE_BOX_X, BATTLE_BOX_Y, BATTLE_BOX_WIDTH, BATTLE_BOX_HEIGHT);

    sprintf(poke_string, "%s  Lvl %d  ", enemy_pok->nickname, enemy_pok->level);
    mvprintw(BATTLE_BOX_ENEMY_Y, BATTLE_BOX_ENEMY_X, poke_string);
    mvprintw(BATTLE_BOX_ENEMY_Y+1, BATTLE_BOX_ENEMY_X, "HP: % 3d/%d ", enemy_pok->currentHP, enemy_pok->maxHP);

    //Display Indication if Pokemon is uncaught
    if (player.is_uncaught_pokemon) {
        mvprintw(BATTLE_BOX_ENEMY_Y, BATTLE_BOX_ENEMY_X - 4, "*");
    }

    mvprintw(BATTLE_BOX_PLAYER_Y, BATTLE_BOX_PLAYER_X, player.name);
    mvprintw(BATTLE_BOX_PLAYER_Y+1, BATTLE_BOX_PLAYER_X, "Safari Balls: %d", safari_ball_count);

    print_btn_instructions(false);

    //Draw text box
    draw_box(TEXT_BOX_X, TEXT_BOX_Y, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);
    refresh();
}