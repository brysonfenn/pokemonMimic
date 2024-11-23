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

void print_safari_battle(int distance);

void setup_safari_zone() {
    player.safari_balls = 30;
    player.bait_count = 30;
}

//Begin a Battle with a pokemon in the safari zone
int safari_zone_encounter(struct Pokemon * enemyPoke) {
    int input_num, last_selection = 0, catch_rate = 0, bait_bonus = 0;
    int distance = 50;
    int flee_chance = 30; //Percentage
    int hit_chance = 50; //
    char eating = 0;
    
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
                

                if (player.safari_balls > 0) {
                    player.safari_balls--;
                    print_safari_battle(distance);
                    text_box_cursors(TEXT_BOX_BEGINNING);
                    printw("%s threw a Safari Ball!", player.name); refresh(); sleep(2);
                    

                    //Ball Hits - Attempt Catch
                    if (rand() % 100 < hit_chance) {
                        //TODO: HANDLE CATCH - Then add pokemone and return
                        text_box_cursors(TEXT_BOX_NEXT_LINE);
                        printw("It hit!"); refresh(); sleep(2);
                        
                        //Catch rate calculations
                        
                        catch_rate = pokemon_get_catch_rate(enemyPoke->id_num) / 3 + bait_bonus;
                        if (catch_rate < 1) catch_rate = 1;

                        //Check if Pokemon was caught
                        text_box_cursors(TEXT_BOX_NEXT_LINE);
                        if (rand() % 256 < catch_rate) {
                            printw("%s was caught!", enemyPoke->name); refresh(); sleep(2);
                            give_pokemon_to_player(enemyPoke); await_user();

                            player.is_battle = false;

                            return BATTLE_WIN;
                        }
                        else {
                            printw("But %s escaped!", enemyPoke->name); refresh(); sleep(2);
                        }

                    }
                    //Ball Misses
                    else {
                        text_box_cursors(TEXT_BOX_BEGINNING);
                        printw("But it Missed!"); refresh(); sleep(2);
                    }
                }
                else {
                    text_box_cursors(TEXT_BOX_BEGINNING);
                    printw("Out of Safari Balls"); refresh(); sleep(2);
                    continue;
                }
                break;
                
            case BAIT:
                //Decrease Flee Chance
                
                
                if (player.bait_count > 0) {
                    player.bait_count--;
                    print_safari_battle(distance);
                    text_box_cursors(TEXT_BOX_BEGINNING);
                    printw("%s threw some bait!", player.name); refresh(); sleep(2);
                    eating = 2;
                    if (flee_chance > 10) {
                        flee_chance -= 10;
                        bait_bonus += 15;
                    }
                }
                else {
                    text_box_cursors(TEXT_BOX_BEGINNING);
                    printw("Out of Bait"); refresh(); sleep(2);
                    continue;
                }

                
                break;
                
            case APPROACH:
                //Increase Flee Chance, Increase Catch Chance
                distance -= 10;
                print_safari_battle(distance);
                text_box_cursors(TEXT_BOX_BEGINNING);
                printw("%s approached %s!", player.name, enemyPoke->nickname); refresh(); sleep(2);
                flee_chance += 10;
                hit_chance += 10;
                if (distance <= 0) {
                    flee_chance = 100;
                }
                break;
                
            case RUN:
                text_box_cursors(TEXT_BOX_BEGINNING);
                printw("Got Away Safely!"); refresh(); await_user();
                player.is_battle = false;
                return BATTLE_WIN;
                break;
            default:
                current_decision = NONE;
                break;
        }

        if ((rand() % 100) < flee_chance) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s fled!", enemyPoke->nickname); refresh(); await_user();
            player.is_battle = false;
            return BATTLE_WIN;
        }
        else if (eating == 2) { 
            eating--;
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s took the Bait!", enemyPoke->nickname); refresh(); sleep(2);
            
            
        }
        else if (eating == 1) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s is still eating the Bait.", enemyPoke->nickname); refresh(); sleep(2);
            eating--;
        }
        else {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s is watching carefully...", enemyPoke->nickname); refresh(); sleep(2);
        }
    }
    ///////// END GET DECISION /////////

    player.is_battle = false;
    return BATTLE_WIN;
}


void print_safari_battle(int distance) {
    Pokemon * enemy_pok = player.enemy_pokemon;
    char print_str[128];

    clear();

    //Draw battle box
    draw_box(BATTLE_BOX_X, BATTLE_BOX_Y, BATTLE_BOX_WIDTH, BATTLE_BOX_HEIGHT);

    mvprintw(BATTLE_BOX_PLAYER_Y, BATTLE_BOX_PLAYER_X, "%s", player.name);
    mvprintw(BATTLE_BOX_PLAYER_Y+1, BATTLE_BOX_PLAYER_X, "Safari Balls: %d -- Bait: %d", player.safari_balls, player.bait_count);

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