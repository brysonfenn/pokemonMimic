#include "attacks.h"

#include <stdint.h>

#include "../player.h"
#include "conditions.h"
#include "../print_defines.h"
#include "../print_utils.h"
#include "pokemon.h"

#define NUM_ATTACKS 50

attack empty_attack=  {"-------------",  0,  0, 100, 100, NO_TYPE,  false, &attack_do_nothing };
//				 	   "Name         "  id  pp  pwr  acc    type     priority    effect         params
attack tackle       = {"Tackle"       ,  1, 35,  40,       95, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack scratch      = {"Scratch"      ,  2, 35,  40,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack growl        = {"Growl"        ,  3, 40,   0,      100, NORMAL,   false, &decrement_opponent_stat, ATTACK_STAT, 100 };
attack tail_whip    = {"Tail Whip"    ,  4, 30,   0,      100, NORMAL,   false, &decrement_opponent_stat, DEFENSE_STAT, 100 };
attack string_shot  = {"String Shot"  ,  5, 40,   0,       95, NORMAL,   false, &decrement_opponent_stat, SPEED_STAT, 100 };
attack poison_sting = {"Poison Sting" ,  6, 35,  25,      100, POISON,   false, &inflict_condition, POISONED, 30 };
attack sand_attack  = {"Sand Attack"  ,  7, 15,   0,      100, NORMAL,   false, &decrement_opponent_stat, ACCURACY_STAT, 100 };
attack quick_attack = {"Quick Attack" ,  8, 30,  40,      100, NORMAL,   true,  &attack_do_nothing, NO_CONDITION, 0 };
attack defense_curl = {"Defense Curl" ,  9, 40,   0,      100, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack vine_whip    = {"Vine whip"    , 10, 25,  45,      100, GRASS,    false, &attack_do_nothing, NO_CONDITION, 0 };

attack leech_seed   = {"Leech seed"   , 11, 10,   0,       90, GRASS,    false, &inflict_condition, SEEDED, 100 };
attack ember        = {"Ember"        , 12, 25,  40,      100, FIRE,     false, &inflict_condition, BURNED, 10 };
attack bubble       = {"Bubble"       , 13, 30,  20,      100, WATER,    false, &decrement_opponent_stat, SPEED_STAT, 10 };
attack poison_powder= {"Poison Powder", 14, 35,   0,       75, POISON,   false, &inflict_condition, POISONED, 100 };
attack sleep_powder = {"Sleep Powder" , 15, 15,   0,       75, GRASS,    false, &inflict_condition, ASLEEP, 100 };
attack razor_leaf   = {"Razor Leaf"   , 16, 25,  55,       95, GRASS,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack metal_claw   = {"Metal Claw"   , 17, 35,  50,       95, STEEL,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack smoke_screen = {"Smoke Screen" , 18, 20,   0,      100, NORMAL,   false, &decrement_opponent_stat, ACCURACY_STAT, 100 };
attack sweet_scent  = {"Sweet Scent"  , 19, 20,   0,  NO_MISS, GRASS,    false, &increment_self_stat, EVASIVENESS_STAT, 100 };
attack growth       = {"Growth"       , 20, 40,   0,  NO_MISS, GRASS,    false, &increment_self_stat, SP_ATTACK_STAT, 100 };

attack scary_face   = {"Scary Face"   , 21, 15,   0,       90, NORMAL,   false, &decrement_opponent_stat2, SPEED_STAT, 100 };
attack flame_thrower= {"Flame Thrower", 22, 15,  95,      100, FIRE,     false, &inflict_condition, BURNED, 10 };
attack slash        = {"Slash"        , 23, 20,  70,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack dragon_rage  = {"Dragon Rage"  , 24, 10,   0,      100, DRAGON,   false, &deal_specific_damage, NO_CONDITION, 40};
attack fire_spin    = {"Fire Spin"    , 25, 15,  15,       70, FIRE,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack wing_attack  = {"Wing Attack"  , 26, 35,  60,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack withdraw     = {"Withdraw"     , 27, 40,   0,  NO_MISS, WATER,    false, &increment_self_stat, DEFENSE_STAT, 100 };
attack water_gun    = {"Water Gun"    , 28, 25,  40,      100, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack bite         = {"Bite"         , 29, 25,  60,      100, DARK,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack rapid_spin   = {"Rapid Spin"   , 30, 40,  20,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };

attack protect      = {"Protect"      , 31, 10,   0,  NO_MISS, NORMAL,   true,  &attack_do_nothing, NO_CONDITION, 0 };
attack skull_bash   = {"Skull Bash"   , 32, 15, 100,      100, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack hydro_pump   = {"Hydro Pump"   , 33,  5, 120,       80, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack harden       = {"Harden"       , 34, 30,   0,  NO_MISS, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack supersonic   = {"Supersonic"   , 35, 35,   0,      100, NORMAL,   false, &inflict_condition, CONFUSED, 100 };
attack confusion    = {"Confusion"    , 36, 25,  50,      100, NORMAL,   false, &inflict_condition, CONFUSED, 10 };
attack stun_spore   = {"Stun Spore"   , 37, 30,   0,       75, GRASS,    false, &inflict_condition, PARALYZED, 100 };

static attack * local_array[NUM_ATTACKS] = { &empty_attack, 
    &tackle, &scratch, &growl, &tail_whip, &string_shot, &poison_sting, &sand_attack, &quick_attack, &defense_curl, &vine_whip,    // #01-10
    &leech_seed, &ember, &bubble, &poison_powder, &sleep_powder, &razor_leaf, &metal_claw, &smoke_screen, &sweet_scent, &growth,   // #11-20
    &scary_face, &flame_thrower, &slash, &dragon_rage, &fire_spin, &wing_attack, &withdraw, &water_gun, &bite, &rapid_spin,        // #21-30
    &protect, &skull_bash, &hydro_pump, &harden, &supersonic, &confusion, &stun_spore };


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
        case SP_ATTACK_STAT:
            sprintf(stat_type_str, "%s", "sp. attack");
            stat_stage = &(pok->sp_atk_stage);
            break;
        case SP_DEFENSE_STAT:
            sprintf(stat_type_str, "%s", "sp. defense");
            stat_stage = &(pok->sp_def_stage);
            break;
        case SPEED_STAT:
            sprintf(stat_type_str, "%s", "speed");
            stat_stage = &(pok->spd_stage);
            break;
        case ACCURACY_STAT:
            sprintf(stat_type_str, "%s", "accuracy");
            stat_stage = &(pok->acc_stage);
            break;
        case EVASIVENESS_STAT:
            sprintf(stat_type_str, "%s", "evasiveness");
            stat_stage = &(pok->evade_stage);
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
            printw("%s's %s ", pok->name, stat_type_str);
            if (stage_number == -1) printw("fell");
            else printw("harshly fell");
            (*stat_stage) += stage_number;

            if (*stat_stage < -6) (*stat_stage) = -6; //Stat stage can be -6 min
        }
        refresh(); sleep(2);
    }
    else if (stage_number > 0) {
        if (pok == player.enemy_pokemon) printw(ENEMY_TEXT);
        if ((*stat_stage) >= 6) {
            printw("%s's %s won't go any higher!", pok->name, stat_type_str);
        }
        else {
            printw("%s's %s ", pok->name, stat_type_str);
            if (stage_number == 1) printw("rose");
            else printw("sharply rose");
            (*stat_stage) += stage_number;

            if (*stat_stage > 6) (*stat_stage) = 6; //Stat stage can be 6 max
        }
        refresh(); sleep(2);
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
    else return 1;
}

//Certain attacks can decrement an enemy pokemon's stat
int decrement_opponent_stat(Condition stat_type, int chance, struct pokemon* victim) {
    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, -1, victim);
        return 0;
    }
    else return 1;
}

int decrement_opponent_stat2(Condition stat_type, int chance, struct pokemon* victim) {
    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, -2, victim);
        return 0;
    }
    else return 1;
}

int deal_specific_damage(Condition nothing, int hp, struct pokemon* victim) {
    victim->currentHP -= hp;
}