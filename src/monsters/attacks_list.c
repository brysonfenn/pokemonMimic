#include "attacks.h"

#include <stdint.h>

#include "../player.h"
#include "conditions.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"
#include "pokemon.h"

#define NUM_ATTACKS 100

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
attack bite         = {"Bite"         , 29, 25,  60,      100, DARK,     false, &inflict_condition, FLINCHED, 30 };
attack rapid_spin   = {"Rapid Spin"   , 30, 40,  20,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };

attack protect      = {"Protect"      , 31, 10,   0,  NO_MISS, NORMAL,   true,  &attack_do_nothing, NO_CONDITION, 0 };
attack skull_bash   = {"Skull Bash"   , 32, 15, 100,      100, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack hydro_pump   = {"Hydro Pump"   , 33,  5, 120,       80, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack harden       = {"Harden"       , 34, 30,   0,  NO_MISS, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack supersonic   = {"Supersonic"   , 35, 35,   0,      100, NORMAL,   false, &inflict_condition, CONFUSED, 100 };
attack confusion    = {"Confusion"    , 36, 25,  50,      100, PSYCHIC,  false, &inflict_condition, CONFUSED, 10 };
attack stun_spore   = {"Stun Spore"   , 37, 30,   0,       75, GRASS,    false, &inflict_condition, PARALYZED, 100 };
attack gust         = {"Gust"         , 38, 35,  40,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack psybeam      = {"Psybeam"      , 39, 20,  65,      100, PSYCHIC,  false, &inflict_condition, CONFUSED, 10 };
attack silver_wind  = {"Silver Wind"  , 40,  5,  60,      100, BUG,      false, &increment_self_stat, SP_ATTACK_STAT, 10 };

attack fury_attack  = {"Fury Attack"  , 41, 20,  15,       85, NORMAL,   false, &hit_multiple_times, 2, 5 };
attack pursuit      = {"Pursuit"      , 42, 10,  40,      100, DARK,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack agility      = {"Agility"      , 43, 20,   0,  NO_MISS, PSYCHIC,  false, &increment_self_stat2, SPEED_STAT, 100 };
attack twineedle    = {"Twineedle"    , 44, 20,  25,      100, BUG,      false, &hit_multiple_times, 2, 2 };
attack pin_missile  = {"Pin Missile"  , 45, 20,  14,       85, BUG,      false, &hit_multiple_times, 2, 5 };
attack hyper_fang   = {"Hyper Fang"   , 46, 15,  80,       90, NORMAL,   false, &inflict_condition, FLINCHED, 10 };
attack super_fang   = {"Super Fang"   , 47, 10,   0,       90, NORMAL,   false, &deal_percentage_damage, NO_CONDITION, 50 };
attack feather_dance= {"Feather Dance", 48, 15,   0,      100, FLYING,   false, &decrement_opponent_stat2, ATTACK_STAT, 100 };
attack swift        = {"Swift"        , 49, 20,  60,  NO_MISS, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack fury_swipes  = {"Fury Swipes"  , 50, 15,  18,       80, NORMAL,   false, &hit_multiple_times, 2, 5 };

attack peck         = {"Peck"         , 51, 35,  35,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack leer         = {"Leer"         , 52, 30,   0,      100, NORMAL,   false, &decrement_opponent_stat, DEFENSE_STAT, 100 };
attack aerial_ace   = {"Aerial Ace"   , 53, 20,  60,  NO_MISS, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack drill_peck   = {"Drill Peck"   , 54, 20,  80,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack thunder_shock= {"Thunder Shock", 55, 30,  40,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 10 };
attack thunder_wave = {"Thunder Wave" , 56, 20,   0,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 100 };
attack double_team  = {"Double Team"  , 57, 15,   0,  NO_MISS, ELECTRIC, false, &increment_self_stat, EVASIVENESS_STAT, 100 };
attack slam         = {"Slam"         , 58, 20,  80,       75, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack thunderbolt  = {"Thunderbolt"  , 59, 15,  95,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 10 };
attack thunder      = {"Thunder"      , 60, 10, 120,       70, ELECTRIC, false, &inflict_condition, PARALYZED, 30 };

attack double_kick  = {"Double Kick"  , 61, 30,  30,      100, FIGHTING, false, &hit_multiple_times, 2, 2 };
attack crunch       = {"Crunch"       , 62, 15,  80,      100, DARK,     false, &decrement_opponent_stat, SP_DEFENSE_STAT, 20 };
attack body_slam    = {"Body Slam"    , 63, 15,  85,      100, NORMAL,   false, &inflict_condition, PARALYZED, 30 };
attack super_power  = {"Super Power"  , 64,  5, 120,      100, FIGHTING, false, &decrement_opponent_stat, ATTACK_STAT, 100 };
attack horn_attack  = {"Horn Attack"  , 65, 25,  65,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack horn_drill   = {"Horn Drill"   , 66,  5,   0,       30, NORMAL,   false, &deal_specific_damage, NO_CONDITION, 1000 };
attack thrash       = {"Thrash"       , 67, 20,  90,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack megahorn     = {"Megahorn"     , 68, 10, 120,       85, BUG,      false, &attack_do_nothing, NO_CONDITION, 0 };
attack pound        = {"Pound"        , 69, 35,  40,       95, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack sing         = {"Sing"         , 70, 15,   0,       55, NORMAL,   false, &inflict_condition, ASLEEP, 100 };

attack double_slap  = {"Double Slap"  , 71, 10,  15,       85, NORMAL,   false, &hit_multiple_times, 2, 5 };
attack minimize     = {"Minimize"     , 72, 20,   0,  NO_MISS, NORMAL,   false, &increment_self_stat, EVASIVENESS_STAT, 100 };
attack cosmic_power = {"Cosmic Power" , 73, 20,   0,  NO_MISS, PSYCHIC,  false, &increment_self_stat, SP_DEFENSE_STAT, 100 };
attack moonlight    = {"Moonlight"    , 74,  5,   0,  NO_MISS, NORMAL,   false, &self_heal, HP_PERCENTAGE, 50 };
attack meteor_mash  = {"Meteor Mash"  , 75, 10, 100,       85, STEEL,    false, &increment_self_stat, ATTACK_STAT, 100 };
attack rock_throw   = {"Rock Throw"   , 76, 15,  50,       90, ROCK,     false, &attack_do_nothing, NO_CONDITION, 0 };



static attack * local_array[NUM_ATTACKS] = { &empty_attack, 
    &tackle, &scratch, &growl, &tail_whip, &string_shot, &poison_sting, &sand_attack, &quick_attack, &defense_curl, &vine_whip,    // #01-10
    &leech_seed, &ember, &bubble, &poison_powder, &sleep_powder, &razor_leaf, &metal_claw, &smoke_screen, &sweet_scent, &growth,   // #11-20
    &scary_face, &flame_thrower, &slash, &dragon_rage, &fire_spin, &wing_attack, &withdraw, &water_gun, &bite, &rapid_spin,        // #21-30
    &protect, &skull_bash, &hydro_pump, &harden, &supersonic, &confusion, &stun_spore, &gust, &psybeam, &silver_wind,              // #31-40
    &fury_attack, &pursuit, &agility, &twineedle, &pin_missile, &hyper_fang, &super_fang, &feather_dance, &swift, &fury_swipes,    // #41-50
    &peck, &leer, &aerial_ace, &drill_peck, &thunder_shock, &thunder_wave, &double_team, &slam, &thunderbolt, &thunder,            // #51-60
    &double_kick, &crunch, &body_slam, &super_power, &horn_attack, &horn_drill, &thrash, &megahorn, &pound, &sing,                 // #61-70
    &double_slap, &minimize, &cosmic_power, &moonlight, &meteor_mash, &rock_throw };


//Return an attack given an attack id number
attack * get_attack_by_id(id_num) {
	return local_array[id_num];
}

//Filler side-effect function that does nothing
int attack_do_nothing(Condition condition, int accuracy, struct Pokemon* pok) {
	return 0;
}

//Change the stat of a pok (stat_type is the type of stat, stage_number is the number of stages to increment (+) or decrement (-))
int change_stat(Condition stat_type, int stage_number, struct Pokemon* pok) {
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
int increment_self_stat(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, 1, self);
        return 0;
    }
    else return 1;
}


//Certain attacks can increment a pokemon's own stat
int increment_self_stat2(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, 2, self);
        return 0;
    }
    else return 1;
}

//Certain attacks can decrement an enemy pokemon's stat
int decrement_opponent_stat(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, -1, victim);
        return 0;
    }
    else return 1;
}

int decrement_opponent_stat2(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, -2, victim);
        return 0;
    }
    else return 1;
}

int deal_specific_damage(Condition nothing, int hp, struct Pokemon* victim, int damage) {
    bool enemy = (victim != player.enemy_pokemon);
    blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);
    victim->currentHP -= hp;
    return 0;
}

//Some attacks hit multiple times
int hit_multiple_times(int min_times, int max_times, struct Pokemon* victim, int damage) {

    bool enemy = (victim != player.enemy_pokemon);
    int rand_times;
    int i = 0;

    //If max times is the same as min times, this is a specific number of times > 1
    if (max_times != min_times) {
        rand_times = (rand() % (max_times - min_times)) + min_times;
        rand_times--;
    }
    else {
        rand_times = min_times;
    }

    // Do not hit again if victim has fainted
    while (victim->currentHP > 0 && i < rand_times) {
        sleep(1);
        blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);
        victim->currentHP -= damage;
        if (victim->currentHP < 0) victim->currentHP = 0;
        i++;
        printBattle();
    }

    sleep(1);
    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("Hit %d time(s)", i+1); refresh(); sleep(2);

    return 0;
}


//Some attacks deal a percentage of damage left
int deal_percentage_damage(Condition nothing, int percent, struct Pokemon* victim, int damage) {
    bool enemy = (victim != player.enemy_pokemon);
    blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);

    float percent_f = ((float) percent) / 100.0;

    int hp = victim->currentHP * percent_f;
    if (hp < 1) hp = 1;
    victim->currentHP -= hp;
}


//Heal a specified percentage (1) or amount (2) of HP
int self_heal(int heal_type, int hp, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    float percentage;

    blinkPokemon(self == player.current_pokemon, HEAL_COLOR, 3);

    switch (heal_type) {
        case HP_PERCENTAGE:
            percentage = (hp / 100.0);
            self->currentHP += (int) (self->maxHP * percentage);
            break;
        case HP_NUM_HP:
            self->currentHP += hp;
            break;
        default:
        text_box_cursors(TEXT_BOX_BEGINNING);
            printw("Unrecognized heal_type"); refresh(); sleep(2);
            return 1;
    }

    if (self->currentHP > self->maxHP) {
        self->currentHP = self->maxHP;
    }
    
    printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("%s restored hp!", self->name); refresh(); sleep(1);

    return 0;
}


//Inflict damage to self
int self_inflict_damage(int damage_type, int hp, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    float percentage;

    blinkPokemon(self == player.current_pokemon, DAMAGED_COLOR, 3);

    switch (damage_type) {
        case HP_PERCENTAGE:
            percentage = (hp / 100.0);
            self->currentHP -= (int) (self->maxHP * percentage);
            break;
        case HP_NUM_HP:
            self->currentHP -= hp;
            break;
        default:
        text_box_cursors(TEXT_BOX_BEGINNING);
            printw("Unrecognized damage_type"); refresh(); sleep(2);
            return 1;
    }

    if (self->currentHP < 0) {
        self->currentHP = 0;
    }
    
    printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("%s took damage too!", self->name); refresh(); sleep(1);

    return 0;
}