#include "attacks_special.h"

#include "pokemon.h"
#include <ncurses.h>

#include "../player.h"

#include "conditions.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"
#include "typings.h"
#include "attacks.h"

int deal_damage(int power, Pokemon * perp, Pokemon * victim, Type move_type) {
    int flags = 0;
    bool enemy;

    if (victim == player.current_pokemon) { enemy = true; }
    else { enemy = false; }

    // Baseline damage
    int damage = get_basic_damage(perp->level, power, perp->baseAttack, victim->baseDefense, 
                                    perp->atk_stage, victim->def_stage);

    damage = (damage <= 0) ? 1 : damage;  // Pokemon should always be able to do 1 damage

    //STAB Bonus
    if (perp->type1 == move_type || perp->type2 == move_type) {
        damage = (damage * 1.5);
    }

    //Calculate effectiveness
    damage = get_damage_after_effectiveness(move_type, victim, damage, true, &flags);

    //Drop HP only if attack has damage power
    if (damage > 0) {
        blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);
        victim->currentHP -= damage;
        if (victim->currentHP < 0) victim->currentHP = 0;
        printBattle();
    }

    if (flags & SUPER_EFFECTIVE_FLAG) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("It's super effective!"); refresh(); sleep(2);
    }
    else if (flags & NOT_VERY_EFFECTIVE_FLAG) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("It's not very effective..."); refresh(); sleep(2);
    }
    else if (flags & DOES_NOT_AFFECT_FLAG) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("It had no effect."); refresh(); sleep(2);
    }

    return ATTACK_SUCCESS;
}


//Speical function for rest move
int rest_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    bool enemy;

    if (victim == player.current_pokemon) {
        self = player.enemy_pokemon;
        enemy = true;
    }
    else {
        self = player.current_pokemon;
        enemy = false;
    }

    self->currentHP = self->maxHP;
    self->visible_condition = ASLEEP;
    self->sleep_count = (rand() % 4) + 1;

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("%s went to sleep to restore health!", self->name); refresh(); sleep(2);
    clear_battle_box(); printBattle();

    return ATTACK_SUCCESS;
}


//Special function for magnitude move
int magnitude_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; }
    else { self = player.current_pokemon; }

    int magnitude_val = 0;
    int power = 0;
    int random = rand() % 100;

    if (random < 5)        { magnitude_val = 4; power = 10; }   //  5%
    else if (random <  15) { magnitude_val = 5; power = 30; }   // 10%
    else if (random <  35) { magnitude_val = 6; power = 50; }   // 20%
    else if (random <  65) { magnitude_val = 7; power = 70; }   // 30%
    else if (random <  85) { magnitude_val = 8; power = 90; }   // 20%
    else if (random <  95) { magnitude_val = 9; power =110; }   // 10%
    else if (random < 100) { magnitude_val =10; power =150; }   //  5%

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("Magnitude %d", magnitude_val); refresh(); sleep(1);

    // Baseline damage
    deal_damage(power, self, victim, GROUND);

    return ATTACK_SUCCESS;
}


//Add repeat condition if not already set 
//  -> return ADDED_REPEATS after first call
//  -> return decremented repeat value (>0) if repeat is already set
//  -> return REMOVED_REPEATS once repeat value reaches 0
int handle_repeats(int repeat_times, struct Pokemon * pok) {

    //Add repeat condition if it is not already there
    if (!has_hidden_condition(pok, REPEAT_MOVE)) {
        add_hidden_condition(pok, REPEAT_MOVE, repeat_times);
        return ADDED_REPEATS;
    }
    //Decrement repeat condition value
    else {
        int repeat_value = decrement_hidden_condition_val(pok, REPEAT_MOVE);
        if (repeat_value == 0) {
            remove_hidden_condition(pok, REPEAT_MOVE);
            return REMOVED_REPEATS;
        }
        return repeat_value;
    }

}


int rollout_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    bool enemy;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; }
    else { self = player.current_pokemon; }

    int repeat_val = handle_repeats(5, self);   //6 times?
    int power;

    if (repeat_val == REMOVED_REPEATS) power = 0;

    if (repeat_val == ADDED_REPEATS) power = 30;
    else power = 30 * (6 - repeat_val);

    deal_damage(power, self, victim, ROCK);

    return ATTACK_SUCCESS;
}

//Special function for uproar move
int uproar_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    bool enemy;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; }
    else { self = player.current_pokemon; }

    int repeat_val = handle_repeats(4, self);   //Repeat 5 times

    deal_damage(50, self, victim, NORMAL);
    if (repeat_val == REMOVED_REPEATS) return ATTACK_FAIL;

    return ATTACK_SUCCESS;
}

//Special function for thrash/outrage moves
int thrash_move_func(int nothing1, int move_type, struct Pokemon * victim, int damage) {
    Pokemon * self;
    bool enemy;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; }
    else { self = player.current_pokemon; }

    int repeat_val = handle_repeats(2, self);   //Repeat 3 times

    deal_damage(90, self, victim, move_type);
    if (repeat_val == REMOVED_REPEATS) {
        inflict_condition(CONFUSED, 100, self, 0);
        return ATTACK_FAIL;
    }

    return ATTACK_SUCCESS;
}


int swagger_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    increment_self_stat2(ATTACK_STAT, 100, self, damage);
    remove_hidden_condition(victim, REPEAT_MOVE);  //Repeat move should be ended
    inflict_condition(CONFUSED, 100, victim, damage);
    return ATTACK_SUCCESS;
}


//Special function for Seismic Toss move
int seismic_toss_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    deal_specific_damage(nothing1, victim->level, victim, damage);
    return ATTACK_SUCCESS;
}


//Special Function for Teleport
int teleport_move_func(int nothing1, int chance, struct Pokemon * victim, int damage) {
    if ((rand() % 100) < chance && !player.trainer_battle) {
        return ATTACK_END_BATTLE;
    }
    else {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("But it failed!"); refresh(); sleep(2);
        return ATTACK_FAIL;
    }
}

//Special Function for Sheer Cold
int k_o_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    int accuracy;
    if (self->level < victim->level) accuracy = 0;
    else accuracy = (self->level - victim->level) + 30;

    if ((rand() % 100) < accuracy) {
        deal_specific_damage(0, victim->currentHP, victim, 0);  //Deal all damage to victim
        return ATTACK_SUCCESS;
    }
    else {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("%s withstood the attack!", victim->name); refresh(); sleep(2);
        return ATTACK_FAIL;
    }
}

//Special Function for Curse
int curse_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    inflict_condition(CURSED, 100, victim, damage);
    self_inflict_damage(HP_PERCENTAGE, 50, victim, damage);
    return ATTACK_SUCCESS;
}

//Special Function for Night Shade
int night_shade_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    deal_specific_damage(0, self->level, victim, 0);
    return ATTACK_SUCCESS;
}

//Special Function for Refresh
int refresh_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    if (self->visible_condition == PARALYZED || self->visible_condition == BURNED || self->visible_condition == POISONED) {
        printw("%s was healed of its condition!", self->name); refresh(); sleep(2);
        self->visible_condition = NO_CONDITION;
        return ATTACK_SUCCESS;
    }
    else {
        printw("It had no effect!"); refresh(); sleep(2);
        return ATTACK_FAIL;
    }
}


//Special function for Flail move
int flail_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; }
    else { self = player.current_pokemon; }

    //Power is dependent on percentage of remaining hp
    int power = 0;
    float hp_percent = ((float) self->currentHP) / 100.0;
    if (hp_percent < 0.05) { power = 200; }
    else if (hp_percent < 0.11) { power = 150; }
    else if (hp_percent < 0.21) { power = 100; }
    else if (hp_percent < 0.36) { power = 80; }
    else if (hp_percent < 0.70) { power = 40; }
    else { power = 20; }

    // Baseline damage
    deal_damage(power, self, victim, NORMAL);

    return ATTACK_SUCCESS;
}

//Special function for Splash move
int splash_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("Nothing Happened!"); refresh(); sleep(2);
}

//Special function for Solar Beam
int solar_beam_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; }
    else { self = player.current_pokemon; }
    int repeat_val = handle_repeats(1, self);   //Repeat once

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    if (repeat_val == ADDED_REPEATS) {
        printw("%s took in sunlight", self->name); refresh(); sleep(2);
    }
    else if (repeat_val == REMOVED_REPEATS) {
        printw("%s released energy absorbed from the Sun!", self->name); refresh(); sleep(2);
        deal_damage(90, self, victim, GRASS);
    }
    
    return ATTACK_SUCCESS;
}

//Special function for Skull Bash
int skull_bash_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; }
    else { self = player.current_pokemon; }
    int repeat_val = handle_repeats(1, self);   //Repeat once

    if (repeat_val == ADDED_REPEATS) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("%s lowered its head", self->name); refresh(); sleep(2);
        increment_self_stat(DEFENSE_STAT, 100, victim, 0);
    }
    else if (repeat_val == REMOVED_REPEATS) {
        deal_damage(100, self, victim, GRASS);
    }
    
    return ATTACK_SUCCESS;
}