#include "attacks.h"

#include <stdint.h>

#include "../player.h"
#include "conditions.h"
#include "../print_defines.h"
#include "../print_utils.h"
#include "pokemon.h"

#define NUM_ATTACKS 50

attack empty_attack=  {"-------------",  0,  0, 100, 100, NO_TYPE,  0.0, false };
//				 	   "Name         "  id  pp  pwr  acc   type     acd  priority    effect         params
attack tackle       = {"Tackle"       ,  1, 35,  40,  95, NORMAL,   0.0, false, &attack_do_nothing };
attack scratch      = {"Scratch"      ,  2, 35,  40, 100, NORMAL,   0.0, false, &attack_do_nothing };
attack growl        = {"Growl"        ,  3, 40,   0, 100, NORMAL,   0.0, false, &decrement_opponent_stat, ATTACK_STAT, 100 };
attack tail_whip    = {"Tail Whip"    ,  4, 30,   0, 100, NORMAL,   0.0, false, &decrement_opponent_stat, DEFENSE_STAT, 100 };
attack string_shot  = {"String Shot"  ,  5, 40,   0,  95, NORMAL,   0.0, false, &decrement_opponent_stat, SPEED_STAT, 100 };
attack poison_sting = {"Poison Sting" ,  6, 35,  25, 100, POISON,   0.0, false, &inflict_condition, POISONED, 100 };
attack sand_attack  = {"Sand Attack"  ,  7, 15,   0, 100, NORMAL,   0.1, false, &attack_do_nothing };
attack quick_attack = {"Quick Attack" ,  8, 30,  40, 100, NORMAL,   0.0,  true, &attack_do_nothing };
attack defense_curl = {"Defense Curl" ,  9, 40,   0, 100, NORMAL,   0.0, false, &increment_self_stat, DEFENSE_STAT, 100 };
attack vine_whip    = {"Vine whip"    , 10, 25,  45, 100, GRASS,    0.0, false, &attack_do_nothing };
attack leech_seed   = {"Leech seed"   , 11, 10,   0,  90, GRASS,    0.0, false, &inflict_condition, SEEDED, 100 };
attack ember        = {"Ember"        , 12, 25,  40, 100, FIRE,     0.0, false, &attack_do_nothing };
attack bubble       = {"Bubble"       , 13, 30,  20, 100, WATER,    0.0, false, &decrement_opponent_stat, SPEED_STAT, 10 };
attack poison_powder= {"Poison Powder", 14, 35,   0,  75, POISON,   0.0, false, &inflict_condition, POISONED, 100 };
attack sleep_powder = {"Sleep Powder" , 15, 35,   0,  75, GRASS,    0.0, false, &inflict_condition, ASLEEP, 100 };
attack razor_leaf   = {"Razor Leaf"   , 16, 25,  55,  95, GRASS,    0.0, false, &attack_do_nothing };
attack metal_claw   = {"Metal Claw"   , 17, 35,  50,  95, FIGHTING, 0.0, false, &attack_do_nothing };
attack smoke_screen = {"Smoke Screen" , 17, 20,   0, 100, NORMAL,   0.1, false, &attack_do_nothing };

static attack * local_array[NUM_ATTACKS] = { &empty_attack, 
    &tackle, &scratch, &growl, &tail_whip, &string_shot, &poison_sting, &sand_attack, &quick_attack, &defense_curl, &vine_whip, // #1-10
    &leech_seed, &ember, &bubble, &poison_powder, &sleep_powder, &razor_leaf, &metal_claw, &smoke_screen }; // #11-17

//Return an attack given an attack id number
attack * get_attack_by_id(id_num) {
	return local_array[id_num];
}

//Change the stat of a pok (stat_type is the type of stat, stage_number is the number of stages to increment (+) or decrement (-))
int change_stat(Condition stat_type, int stage_number, struct pokemon* pok) {
    char stat_type_str[50];
    int16_t * stat_stage;

    switch (stat_type) {
         case ATTACK_STAT:
            sprintf(stat_type_str, "%s", "attack");
            stat_stage = &(pok->atk_stage);
            break;
         case DEFENSE_STAT:
            sprintf(stat_type_str, "%s", "defense");
            stat_stage = &(pok->def_stage);
            break;
        case SPEED_STAT:
            sprintf(stat_type_str, "%s", "speed");
            stat_stage = &(pok->spd_stage);
            break;
        default:
            break;
    }

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    
    //Drop Stat unless we are already 6 stages below
    if (stage_number < 0) {
        if (pok == player.enemy_pokemon) printw(ENEMY_TEXT);
        if ((*stat_stage) <= -6) {
            printw("%s's %s won't go any lower!", pok->name, stat_type_str);
        }
        else {
            printw("%s's %s fell", pok->name, stat_type_str);
            (*stat_stage) += stage_number;
        }
        refresh(); sleep(1);
    }
    else if (stage_number > 0) {
        if (pok == player.enemy_pokemon) printw(ENEMY_TEXT);
        if ((*stat_stage) >= 6) {
            printw("%s's %s won't go any higher!", pok->name, stat_type_str);
        }
        else {
            printw("%s's %s rose", pok->name, stat_type_str); 
            (*stat_stage) += stage_number;
        }
        refresh(); sleep(1);
    }
}

//Certain attacks can increment a pokemon's own stat
int increment_self_stat(Condition stat_type, int chance, struct pokemon* victim) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, 1, self);
        return 0;
    }
    else {
        return 1;
    }
}

//Certain attacks can decrement an enemy pokemon's stat
int decrement_opponent_stat(Condition stat_type, int chance, struct pokemon* victim) {
    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, -1, victim);
        return 0;
    }
    else {
        return 1;
    }
}