#include "motion2d.h"

#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "location.h"
#include "maps.h"
#include "map_drawing.h"
#include "doors.h"
#include "selectables.h"

#include "../battles/trainer.h"
#include "../battles/wild_pokemon.h"
#include "../battles/battle.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../player.h"
#include "../items/items.h"
#include "../poke_center.h"


//Function pointers
typedef void (*init_map_func) ();

static init_map_func draw_map;

static char player_char = PLAYER_MOVING_DOWN;

static char * player_y;
static char * player_x;

// Forward Declarations
void init_map();
void handle_actions(int action_id);
bool is_movable_space(int yInc, int xInc);

static int leave_msg_count = 0;

// Draw the current map to the screen and handle player motion, battles, etc until user returns to the menu
void handle_motion() {
    char print_str[512];

    player_y = &(player.loc->y);
    player_x = &(player.loc->x);

    Selectable * selectable_ptr;
    Trainer * trainer_ptr;
    char * message_ptr;
    int return_value;

    change_map(player.loc->map, *player_x, *player_y);

    int ch;
    char next_char;

    while (1) {
        flushinp();
        if ((ch = getch()) == MENU_CHAR) break;
        
        mvaddch(*player_y, *player_x, ' '); 
        switch (ch) {
        	case KEY_UP:
                if (player_char != PLAYER_MOVING_UP) player_char = PLAYER_MOVING_UP;
                else if (*player_y > 1 && is_movable_space(-1,0)) (*player_y)--;
                break;

            case KEY_DOWN:
                next_char = mvinch(*player_y+1, *player_x);
                if (player_char != PLAYER_MOVING_DOWN) player_char = PLAYER_MOVING_DOWN;
                //If next space is a fence, jump it
                else if (next_char == '-') {
                    attrset(COLOR_PAIR(PLAYER_COLOR));
                    mvaddch(++(*player_y), *player_x, player_char); refresh();
                    usleep(300000);
                    mvaddch(++(*player_y), *player_x, player_char); refresh();
                    attrset(COLOR_PAIR(DEFAULT_COLOR));
                    mvaddch((*player_y-1), *player_x, '-');
                    flushinp(); continue;
                }
                //Else just move one down
            	else if (*player_y < 20 && is_movable_space(1,0)) (*player_y)++;
                break;

            case KEY_LEFT:
                if (player_char != PLAYER_MOVING_LEFT) player_char = PLAYER_MOVING_LEFT;
                //Move only one space if next space is surrounded (door);
                else if (*player_x > 1 && !is_movable_space(-1,-1) && !is_movable_space(1,-1) && is_movable_space(0,-1)) 
                    (*player_x)--;
                //Default movement for horizontal is 1 spaces
                else if (*player_x > 1 && is_movable_space(0,-1) && is_movable_space(0,-2)) (*player_x)-=1;
                //Only move one space if there is only one space to move
                else if (*player_x > 1 && is_movable_space(0,-1)) (*player_x)--;
                break;

            case KEY_RIGHT:
                if (player_char != PLAYER_MOVING_RIGHT) player_char = PLAYER_MOVING_RIGHT;
                //Move only one space if next space is surrounded (door);
                else if (*player_x < 100 && !is_movable_space(-1,1) && !is_movable_space(1,1) && is_movable_space(0,1)) 
                    (*player_x)++;
                //Default movement for horizontal is 1 spaces
                else if (*player_x < 100 && is_movable_space(0,1) && is_movable_space(0,2)) (*player_x)+=1;
                //Only move one space if there is only one space to move
                else if (*player_x < 100 && is_movable_space(0,1)) (*player_x)++;
                break;

            case SELECT_CHAR:
                //Redraw player
                attrset(COLOR_PAIR(PLAYER_COLOR)); mvaddch(*player_y, *player_x, player_char); 
                attrset(COLOR_PAIR(DEFAULT_COLOR));

                selectable_ptr = get_selectable(*player_x, *player_y, player_char);
                if (selectable_ptr->x == 0) continue;
                if (selectable_ptr->is_trainer) {
                    trainer_ptr = (Trainer *) selectable_ptr->data;
                    if (already_battled_trainer(trainer_ptr->id_num)) {
                        print_to_message_box("\"We already battled\"");
                        continue;
                    }
                    
                    //Print trainer's battle message
                    sprintf(print_str, "\"%s\"", trainer_ptr->message);
                    print_to_message_box(print_str); sleep(2);

                    //Blink screen and reset screen without battle message
                    save_print_state();
                    blink_screen(5, restore_print_state);
                    begin_message_box(); save_print_state();
                    
                    //Handle trainer battle and return values from that function
                    return_value = battle_trainer(trainer_ptr);
                    if (return_value == BATTLE_WHITE_OUT) {
                        init_map(); //Reset map to pokemon center map if battle white out
                    }
                    else {
                        restore_print_state();
                    }
                    continue;
                }
                else {
                    message_ptr = (char *) selectable_ptr->data;
                    if (strcmp(message_ptr,"NONE") == 0) continue;
                    print_to_message_box(message_ptr);
                }
                break;
            default:
                attrset(COLOR_PAIR(PLAYER_COLOR));
                mvaddch(*player_y, *player_x, player_char);
                attrset(COLOR_PAIR(DEFAULT_COLOR));
                continue;
        }

        //Leave printed message for 5 movements
        if (leave_msg_count < 5) leave_msg_count++;
        else begin_message_box();

        draw_static_elements();

        bool hitGrass = ((mvinch(*player_y, *player_x) & A_CHARTEXT) == GRASS_CHAR) && !(leave_msg_count < 5);
        int random = rand() % 100;

        // Set player color, move, and unset
        attrset(COLOR_PAIR(PLAYER_COLOR));
        mvaddch(*player_y, *player_x, player_char);
        attrset(COLOR_PAIR(DEFAULT_COLOR));

        //Display player location
        // sprintf(print_str, "Player location (%d,%d)", *player_x, *player_y);
        // mvprintw(21,4, print_str); 

        refresh();

        if (hitGrass && (random < 10)) {
            save_print_state();
            blink_screen(5, restore_print_state);
            begin_message_box(); save_print_state();
            battle_wild_pokemon();
            init_map();
            continue;
        }

        // Check if action or portal needs to be handled
        Location door = *(get_door(*player_x, *player_y));
        int action = door.action;

        //Handle portal to another map
        if (action == -1) {
            usleep(300000);
            change_map(door.next_map, door.next_x, door.next_y);
            continue;
        }
        else if (action != 0) {
            usleep(300000);
            handle_actions(action);
            player_char = PLAYER_MOVING_DOWN;
            init_map();
            continue;
        }

        // Handle trainers that are in range to see the player //
        int x_inc, y_inc;
        int * trainer_x;
        int * trainer_y;
        char trainer_ch;

        Selectable * curr_sel = get_triggered_selectable(*player_x, *player_y, &x_inc, &y_inc);
        if (curr_sel->x != 0) {
            trainer_ptr = (Trainer *) curr_sel->data;
            trainer_x = &(curr_sel->x);
            trainer_y = &(curr_sel->y);
            trainer_ch = mvinch(*trainer_y, *trainer_x);
            
            while ((mvinch(*trainer_y + y_inc, *trainer_x + x_inc) & A_CHARTEXT) == ' ') {
                attrset(COLOR_PAIR(TRAINER_COLOR));
                mvaddch(*trainer_y, *trainer_x, ' ');
                *trainer_y += y_inc; *trainer_x += x_inc;
                mvaddch(*trainer_y, *trainer_x, trainer_ch);
                attrset(COLOR_PAIR(DEFAULT_COLOR));
                refresh();
                usleep(100000);
            }
            save_print_state();

            //Print trainer's battle message
            sprintf(print_str, "\"%s\"", trainer_ptr->message);
            print_to_message_box(print_str); sleep(2);


            //Blink screen and reset screen without battle message
            save_print_state();
            blink_screen(5, restore_print_state);
            begin_message_box(); save_print_state();

            //Handle trainer battle and return values        
            return_value = battle_trainer(trainer_ptr);

            if (return_value == BATTLE_WHITE_OUT) {
                init_map();
                continue;
            }
            else {
                restore_print_state();
                print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true); 
                continue;
            }
        }
    }
}


//Change and redraw the map and player
void change_map(int map, int x, int y) {
    clear();
    clear_doors();
    clear_selectables();
    usleep(10000);

    player.loc->x = x;
    player.loc->y = y;
    player.loc->map = map;
    change_map_funcs(map, &draw_map);

    init_map();
}


//Check if a given space movement would result in a collision
bool is_movable_space(int yInc, int xInc) {
    char next_tile = mvinch(*player_y+yInc, *player_x+xInc);
    if (next_tile == ' ' || next_tile == GRASS_CHAR)
        return true;
    else
        return false;
}


//Draw map and player onto the screen
void init_map() {
    clear();
    draw_map();
    print_btn_instructions(MAP_X+MAP_WIDTH+2, TEXT_BOX_Y, true);
    draw_static_elements();
    draw_box(MAP_X,MAP_Y+MAP_HEIGHT,MAP_WIDTH,5); //Draw message box
    leave_msg_count = 0;    //leave map label for 5 movements

    attrset(COLOR_PAIR(PLAYER_COLOR));
    mvaddch(*player_y, *player_x, player_char);
    attrset(COLOR_PAIR(DEFAULT_COLOR));

    refresh();
}


//Handle all map actions
void handle_actions(int action_id) {
    switch (action_id) {
        case MART_ACTION:
            while (handle_mart() == ITEM_FAILURE) { clear(); }
            player.loc->y += 1; //Set player location to outside
            break;
        case POKE_CENTER_ACTION:
            handle_poke_center();
            player.loc->y += 1; //Set player location to outside
            break;
        default:
            break;
    }
    clear();
}