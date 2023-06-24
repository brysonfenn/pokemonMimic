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

void deal_damage(int power, Pokemon * perp, Pokemon * victim, int move_type) {
    int flags;
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
    printw("%s went to sleep!", self->name); refresh(); sleep(2);
    clear_battle_box(); printBattle();

    return 0;
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
}


//Add repeat condition if not already set -> return 0
//Get repeat value if repeat is already set
int handle_repeats(int repeat_times, struct Pokemon * pok) {

    //Add repeat condition if it is not already there
    if (!has_hidden_condition(pok, REPEAT_MOVE)) {
        add_hidden_condition(pok, REPEAT_MOVE, repeat_times);
        return ADDED_REPEATS;
    }
    //Decreement repeat condition value
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

    int repeat_val = handle_repeats(5, self);
    int power;

    if (repeat_val == REMOVED_REPEATS) return 1;
    else if (repeat_val == ADDED_REPEATS) power = 30;
    else power = 30 * (6 - repeat_val);

    

    deal_damage(power, self, victim, ROCK);

    return 0;
}


int swagger_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    increment_self_stat2(ATTACK_STAT, 100, self, damage);
    remove_hidden_condition(victim, REPEAT_MOVE);  //Repeat move should be ended
    inflict_condition(CONFUSED, 100, victim, damage);
    return 0;
}


//Special function for Seismic Toss move
int seismic_toss_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    deal_specific_damage(nothing1, victim->level, victim, damage);
}